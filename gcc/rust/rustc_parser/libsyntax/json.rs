//! A JSON emitter for errors.
//!
//! This works by converting errors to a simplified structural format (see the
//! structs at the start of the file) and then serializing them. These should
//! contain as much information about the error as possible.
//!
//! The format of the JSON output should be considered *unstable*. For now the
//! structs at the end of this file (Diagnostic*) specify the error format.

// FIXME: spec the JSON output properly.

use crate::source_map::{SourceMap, FilePathMapping};

use errors::registry::Registry;
use errors::{DiagnosticBuilder, SubDiagnostic, CodeSuggestion, SourceMapper};
use errors::{DiagnosticId, Applicability};
use errors::emitter::{Emitter, HumanReadableErrorType};

use syntax_pos::{MacroBacktrace, Span, SpanLabel, MultiSpan};
use rustc_data_structures::sync::{self, Lrc};
use std::io::{self, Write};
use std::path::Path;
use std::vec;
use std::sync::{Arc, Mutex};

use rustc_serialize::json::{as_json, as_pretty_json};

pub struct JsonEmitter {
    dst: Box<dyn Write + Send>,
    registry: Option<Registry>,
    sm: Lrc<dyn SourceMapper + sync::Send + sync::Sync>,
    pretty: bool,
    ui_testing: bool,
    json_rendered: HumanReadableErrorType,
}

impl JsonEmitter {
    pub fn stderr(
        registry: Option<Registry>,
        source_map: Lrc<SourceMap>,
        pretty: bool,
        json_rendered: HumanReadableErrorType,
    ) -> JsonEmitter {
        JsonEmitter {
            dst: Box::new(io::stderr()),
            registry,
            sm: source_map,
            pretty,
            ui_testing: false,
            json_rendered,
        }
    }

    pub fn basic(pretty: bool, json_rendered: HumanReadableErrorType) -> JsonEmitter {
        let file_path_mapping = FilePathMapping::empty();
        JsonEmitter::stderr(None, Lrc::new(SourceMap::new(file_path_mapping)),
                            pretty, json_rendered)
    }

    pub fn new(
        dst: Box<dyn Write + Send>,
        registry: Option<Registry>,
        source_map: Lrc<SourceMap>,
        pretty: bool,
        json_rendered: HumanReadableErrorType,
    ) -> JsonEmitter {
        JsonEmitter {
            dst,
            registry,
            sm: source_map,
            pretty,
            ui_testing: false,
            json_rendered,
        }
    }

    pub fn ui_testing(self, ui_testing: bool) -> Self {
        Self { ui_testing, ..self }
    }
}

impl Emitter for JsonEmitter {
    fn emit_diagnostic(&mut self, db: &DiagnosticBuilder<'_>) {
        let data = Diagnostic::from_diagnostic_builder(db, self);
        let result = if self.pretty {
            writeln!(&mut self.dst, "{}", as_pretty_json(&data))
        } else {
            writeln!(&mut self.dst, "{}", as_json(&data))
        };
        if let Err(e) = result {
            panic!("failed to print diagnostics: {:?}", e);
        }
    }

    fn emit_artifact_notification(&mut self, path: &Path, artifact_type: &str) {
        let data = ArtifactNotification { artifact: path, emit: artifact_type };
        let result = if self.pretty {
            writeln!(&mut self.dst, "{}", as_pretty_json(&data))
        } else {
            writeln!(&mut self.dst, "{}", as_json(&data))
        };
        if let Err(e) = result {
            panic!("failed to print notification: {:?}", e);
        }
    }
}

// The following data types are provided just for serialisation.

#[derive(RustcEncodable)]
struct Diagnostic {
    /// The primary error message.
    message: String,
    code: Option<DiagnosticCode>,
    /// "error: internal compiler error", "error", "warning", "note", "help".
    level: &'static str,
    spans: Vec<DiagnosticSpan>,
    /// Associated diagnostic messages.
    children: Vec<Diagnostic>,
    /// The message as rustc would render it.
    rendered: Option<String>,
}

#[derive(RustcEncodable)]
#[allow(unused_attributes)]
struct DiagnosticSpan {
    file_name: String,
    byte_start: u32,
    byte_end: u32,
    /// 1-based.
    line_start: usize,
    line_end: usize,
    /// 1-based, character offset.
    column_start: usize,
    column_end: usize,
    /// Is this a "primary" span -- meaning the point, or one of the points,
    /// where the error occurred?
    is_primary: bool,
    /// Source text from the start of line_start to the end of line_end.
    text: Vec<DiagnosticSpanLine>,
    /// Label that should be placed at this location (if any)
    label: Option<String>,
    /// If we are suggesting a replacement, this will contain text
    /// that should be sliced in atop this span.
    suggested_replacement: Option<String>,
    /// If the suggestion is approximate
    suggestion_applicability: Option<Applicability>,
    /// Macro invocations that created the code at this span, if any.
    expansion: Option<Box<DiagnosticSpanMacroExpansion>>,
}

#[derive(RustcEncodable)]
struct DiagnosticSpanLine {
    text: String,

    /// 1-based, character offset in self.text.
    highlight_start: usize,

    highlight_end: usize,
}

#[derive(RustcEncodable)]
struct DiagnosticSpanMacroExpansion {
    /// span where macro was applied to generate this code; note that
    /// this may itself derive from a macro (if
    /// `span.expansion.is_some()`)
    span: DiagnosticSpan,

    /// name of macro that was applied (e.g., "foo!" or "#[derive(Eq)]")
    macro_decl_name: String,

    /// span where macro was defined (if known)
    def_site_span: Option<DiagnosticSpan>,
}

#[derive(RustcEncodable)]
struct DiagnosticCode {
    /// The code itself.
    code: String,
    /// An explanation for the code.
    explanation: Option<&'static str>,
}

#[derive(RustcEncodable)]
struct ArtifactNotification<'a> {
    /// The path of the artifact.
    artifact: &'a Path,
    /// What kind of artifact we're emitting.
    emit: &'a str,
}

impl Diagnostic {
    fn from_diagnostic_builder(db: &DiagnosticBuilder<'_>,
                               je: &JsonEmitter)
                               -> Diagnostic {
        let sugg = db.suggestions.iter().map(|sugg| {
            Diagnostic {
                message: sugg.msg.clone(),
                code: None,
                level: "help",
                spans: DiagnosticSpan::from_suggestion(sugg, je),
                children: vec![],
                rendered: None,
            }
        });

        // generate regular command line output and store it in the json

        // A threadsafe buffer for writing.
        #[derive(Default, Clone)]
        struct BufWriter(Arc<Mutex<Vec<u8>>>);

        impl Write for BufWriter {
            fn write(&mut self, buf: &[u8]) -> io::Result<usize> {
                self.0.lock().unwrap().write(buf)
            }
            fn flush(&mut self) -> io::Result<()> {
                self.0.lock().unwrap().flush()
            }
        }
        let buf = BufWriter::default();
        let output = buf.clone();
        je.json_rendered.new_emitter(Box::new(buf), Some(je.sm.clone()), false)
            .ui_testing(je.ui_testing).emit_diagnostic(db);
        let output = Arc::try_unwrap(output.0).unwrap().into_inner().unwrap();
        let output = String::from_utf8(output).unwrap();

        Diagnostic {
            message: db.message(),
            code: DiagnosticCode::map_opt_string(db.code.clone(), je),
            level: db.level.to_str(),
            spans: DiagnosticSpan::from_multispan(&db.span, je),
            children: db.children.iter().map(|c| {
                Diagnostic::from_sub_diagnostic(c, je)
            }).chain(sugg).collect(),
            rendered: Some(output),
        }
    }

    fn from_sub_diagnostic(db: &SubDiagnostic, je: &JsonEmitter) -> Diagnostic {
        Diagnostic {
            message: db.message(),
            code: None,
            level: db.level.to_str(),
            spans: db.render_span.as_ref()
                     .map(|sp| DiagnosticSpan::from_multispan(sp, je))
                     .unwrap_or_else(|| DiagnosticSpan::from_multispan(&db.span, je)),
            children: vec![],
            rendered: None,
        }
    }
}

impl DiagnosticSpan {
    fn from_span_label(span: SpanLabel,
                       suggestion: Option<(&String, Applicability)>,
                       je: &JsonEmitter)
                       -> DiagnosticSpan {
        Self::from_span_etc(span.span,
                            span.is_primary,
                            span.label,
                            suggestion,
                            je)
    }

    fn from_span_etc(span: Span,
                     is_primary: bool,
                     label: Option<String>,
                     suggestion: Option<(&String, Applicability)>,
                     je: &JsonEmitter)
                     -> DiagnosticSpan {
        // obtain the full backtrace from the `macro_backtrace`
        // helper; in some ways, it'd be better to expand the
        // backtrace ourselves, but the `macro_backtrace` helper makes
        // some decision, such as dropping some frames, and I don't
        // want to duplicate that logic here.
        let backtrace = span.macro_backtrace().into_iter();
        DiagnosticSpan::from_span_full(span,
                                       is_primary,
                                       label,
                                       suggestion,
                                       backtrace,
                                       je)
    }

    fn from_span_full(span: Span,
                      is_primary: bool,
                      label: Option<String>,
                      suggestion: Option<(&String, Applicability)>,
                      mut backtrace: vec::IntoIter<MacroBacktrace>,
                      je: &JsonEmitter)
                      -> DiagnosticSpan {
        let start = je.sm.lookup_char_pos(span.lo());
        let end = je.sm.lookup_char_pos(span.hi());
        let backtrace_step = backtrace.next().map(|bt| {
            let call_site =
                Self::from_span_full(bt.call_site,
                                     false,
                                     None,
                                     None,
                                     backtrace,
                                     je);
            let def_site_span = bt.def_site_span.map(|sp| {
                Self::from_span_full(sp,
                                     false,
                                     None,
                                     None,
                                     vec![].into_iter(),
                                     je)
            });
            Box::new(DiagnosticSpanMacroExpansion {
                span: call_site,
                macro_decl_name: bt.macro_decl_name,
                def_site_span,
            })
        });

        DiagnosticSpan {
            file_name: start.file.name.to_string(),
            byte_start: span.lo().0 - start.file.start_pos.0,
            byte_end: span.hi().0 - start.file.start_pos.0,
            line_start: start.line,
            line_end: end.line,
            column_start: start.col.0 + 1,
            column_end: end.col.0 + 1,
            is_primary,
            text: DiagnosticSpanLine::from_span(span, je),
            suggested_replacement: suggestion.map(|x| x.0.clone()),
            suggestion_applicability: suggestion.map(|x| x.1),
            expansion: backtrace_step,
            label,
        }
    }

    fn from_multispan(msp: &MultiSpan, je: &JsonEmitter) -> Vec<DiagnosticSpan> {
        msp.span_labels()
           .into_iter()
           .map(|span_str| Self::from_span_label(span_str, None, je))
           .collect()
    }

    fn from_suggestion(suggestion: &CodeSuggestion, je: &JsonEmitter)
                       -> Vec<DiagnosticSpan> {
        suggestion.substitutions
                      .iter()
                      .flat_map(|substitution| {
                          substitution.parts.iter().map(move |suggestion_inner| {
                              let span_label = SpanLabel {
                                  span: suggestion_inner.span,
                                  is_primary: true,
                                  label: None,
                              };
                              DiagnosticSpan::from_span_label(span_label,
                                                              Some((&suggestion_inner.snippet,
                                                                   suggestion.applicability)),
                                                              je)
                          })
                      })
                      .collect()
    }
}

impl DiagnosticSpanLine {
    fn line_from_source_file(fm: &syntax_pos::SourceFile,
                         index: usize,
                         h_start: usize,
                         h_end: usize)
                         -> DiagnosticSpanLine {
        DiagnosticSpanLine {
            text: fm.get_line(index).map_or(String::new(), |l| l.into_owned()),
            highlight_start: h_start,
            highlight_end: h_end,
        }
    }

    /// Creates a list of DiagnosticSpanLines from span - each line with any part
    /// of `span` gets a DiagnosticSpanLine, with the highlight indicating the
    /// `span` within the line.
    fn from_span(span: Span, je: &JsonEmitter) -> Vec<DiagnosticSpanLine> {
        je.sm.span_to_lines(span)
            .map(|lines| {
                let fm = &*lines.file;
                lines.lines
                    .iter()
                    .map(|line| DiagnosticSpanLine::line_from_source_file(
                        fm,
                        line.line_index,
                        line.start_col.0 + 1,
                        line.end_col.0 + 1,
                    )).collect()
            }).unwrap_or_else(|_| vec![])
    }
}

impl DiagnosticCode {
    fn map_opt_string(s: Option<DiagnosticId>, je: &JsonEmitter) -> Option<DiagnosticCode> {
        s.map(|s| {
            let s = match s {
                DiagnosticId::Error(s) => s,
                DiagnosticId::Lint(s) => s,
            };
            let explanation = je.registry
                                .as_ref()
                                .and_then(|registry| registry.find_description(&s));

            DiagnosticCode {
                code: s,
                explanation,
            }
        })
    }
}
