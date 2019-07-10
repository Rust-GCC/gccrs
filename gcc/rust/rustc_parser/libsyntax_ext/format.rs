use ArgumentType::*;
use Position::*;

use fmt_macros as parse;

use errors::DiagnosticBuilder;
use errors::Applicability;

use syntax::ast;
use syntax::ext::base::{self, *};
use syntax::ext::build::AstBuilder;
use syntax::parse::token;
use syntax::ptr::P;
use syntax::symbol::{Symbol, sym};
use syntax::tokenstream;
use syntax_pos::{MultiSpan, Span, DUMMY_SP};

use rustc_data_structures::fx::{FxHashMap, FxHashSet};
use std::borrow::Cow;
use std::collections::hash_map::Entry;

#[derive(PartialEq)]
enum ArgumentType {
    Placeholder(String),
    Count,
}

enum Position {
    Exact(usize),
    Named(Symbol),
}

struct Context<'a, 'b> {
    ecx: &'a mut ExtCtxt<'b>,
    /// The macro's call site. References to unstable formatting internals must
    /// use this span to pass the stability checker.
    macsp: Span,
    /// The span of the format string literal.
    fmtsp: Span,

    /// List of parsed argument expressions.
    /// Named expressions are resolved early, and are appended to the end of
    /// argument expressions.
    ///
    /// Example showing the various data structures in motion:
    ///
    /// * Original: `"{foo:o} {:o} {foo:x} {0:x} {1:o} {:x} {1:x} {0:o}"`
    /// * Implicit argument resolution: `"{foo:o} {0:o} {foo:x} {0:x} {1:o} {1:x} {1:x} {0:o}"`
    /// * Name resolution: `"{2:o} {0:o} {2:x} {0:x} {1:o} {1:x} {1:x} {0:o}"`
    /// * `arg_types` (in JSON): `[[0, 1, 0], [0, 1, 1], [0, 1]]`
    /// * `arg_unique_types` (in simplified JSON): `[["o", "x"], ["o", "x"], ["o", "x"]]`
    /// * `names` (in JSON): `{"foo": 2}`
    args: Vec<P<ast::Expr>>,
    /// Placeholder slot numbers indexed by argument.
    arg_types: Vec<Vec<usize>>,
    /// Unique format specs seen for each argument.
    arg_unique_types: Vec<Vec<ArgumentType>>,
    /// Map from named arguments to their resolved indices.
    names: FxHashMap<Symbol, usize>,

    /// The latest consecutive literal strings, or empty if there weren't any.
    literal: String,

    /// Collection of the compiled `rt::Argument` structures
    pieces: Vec<P<ast::Expr>>,
    /// Collection of string literals
    str_pieces: Vec<P<ast::Expr>>,
    /// Stays `true` if all formatting parameters are default (as in "{}{}").
    all_pieces_simple: bool,

    /// Mapping between positional argument references and indices into the
    /// final generated static argument array. We record the starting indices
    /// corresponding to each positional argument, and number of references
    /// consumed so far for each argument, to facilitate correct `Position`
    /// mapping in `build_piece`. In effect this can be seen as a "flattened"
    /// version of `arg_unique_types`.
    ///
    /// Again with the example described above in docstring for `args`:
    ///
    /// * `arg_index_map` (in JSON): `[[0, 1, 0], [2, 3, 3], [4, 5]]`
    arg_index_map: Vec<Vec<usize>>,

    /// Starting offset of count argument slots.
    count_args_index_offset: usize,

    /// Count argument slots and tracking data structures.
    /// Count arguments are separately tracked for de-duplication in case
    /// multiple references are made to one argument. For example, in this
    /// format string:
    ///
    /// * Original: `"{:.*} {:.foo$} {1:.*} {:.0$}"`
    /// * Implicit argument resolution: `"{1:.0$} {2:.foo$} {1:.3$} {4:.0$}"`
    /// * Name resolution: `"{1:.0$} {2:.5$} {1:.3$} {4:.0$}"`
    /// * `count_positions` (in JSON): `{0: 0, 5: 1, 3: 2}`
    /// * `count_args`: `vec![Exact(0), Exact(5), Exact(3)]`
    count_args: Vec<Position>,
    /// Relative slot numbers for count arguments.
    count_positions: FxHashMap<usize, usize>,
    /// Number of count slots assigned.
    count_positions_count: usize,

    /// Current position of the implicit positional arg pointer, as if it
    /// still existed in this phase of processing.
    /// Used only for `all_pieces_simple` tracking in `build_piece`.
    curarg: usize,
    /// Current piece being evaluated, used for error reporting.
    curpiece: usize,
    /// Keep track of invalid references to positional arguments.
    invalid_refs: Vec<(usize, usize)>,
    /// Spans of all the formatting arguments, in order.
    arg_spans: Vec<Span>,
    /// Whether this formatting string is a literal or it comes from a macro.
    is_literal: bool,
}

/// Parses the arguments from the given list of tokens, returning the diagnostic
/// if there's a parse error so we can continue parsing other format!
/// expressions.
///
/// If parsing succeeds, the return value is:
///
/// ```text
/// Some((fmtstr, parsed arguments, index map for named arguments))
/// ```
fn parse_args<'a>(
    ecx: &mut ExtCtxt<'a>,
    sp: Span,
    tts: &[tokenstream::TokenTree]
) -> Result<(P<ast::Expr>, Vec<P<ast::Expr>>, FxHashMap<Symbol, usize>), DiagnosticBuilder<'a>> {
    let mut args = Vec::<P<ast::Expr>>::new();
    let mut names = FxHashMap::<Symbol, usize>::default();

    let mut p = ecx.new_parser_from_tts(tts);

    if p.token == token::Eof {
        return Err(ecx.struct_span_err(sp, "requires at least a format string argument"));
    }

    let fmtstr = p.parse_expr()?;
    let mut named = false;

    while p.token != token::Eof {
        if !p.eat(&token::Comma) {
            return Err(ecx.struct_span_err(p.token.span, "expected token: `,`"));
        }
        if p.token == token::Eof {
            break;
        } // accept trailing commas
        if named || (p.token.is_ident() && p.look_ahead(1, |t| *t == token::Eq)) {
            named = true;
            let name = if let token::Ident(name, _) = p.token.kind {
                p.bump();
                name
            } else {
                return Err(ecx.struct_span_err(
                    p.token.span,
                    "expected ident, positional arguments cannot follow named arguments",
                ));
            };

            p.expect(&token::Eq)?;
            let e = p.parse_expr()?;
            if let Some(prev) = names.get(&name) {
                ecx.struct_span_err(e.span, &format!("duplicate argument named `{}`", name))
                    .span_note(args[*prev].span, "previously here")
                    .emit();
                continue;
            }

            // Resolve names into slots early.
            // Since all the positional args are already seen at this point
            // if the input is valid, we can simply append to the positional
            // args. And remember the names.
            let slot = args.len();
            names.insert(name, slot);
            args.push(e);
        } else {
            let e = p.parse_expr()?;
            args.push(e);
        }
    }
    Ok((fmtstr, args, names))
}

impl<'a, 'b> Context<'a, 'b> {
    fn resolve_name_inplace(&self, p: &mut parse::Piece<'_>) {
        // NOTE: the `unwrap_or` branch is needed in case of invalid format
        // arguments, e.g., `format_args!("{foo}")`.
        let lookup = |s: Symbol| *self.names.get(&s).unwrap_or(&0);

        match *p {
            parse::String(_) => {}
            parse::NextArgument(ref mut arg) => {
                if let parse::ArgumentNamed(s) = arg.position {
                    arg.position = parse::ArgumentIs(lookup(s));
                }
                if let parse::CountIsName(s) = arg.format.width {
                    arg.format.width = parse::CountIsParam(lookup(s));
                }
                if let parse::CountIsName(s) = arg.format.precision {
                    arg.format.precision = parse::CountIsParam(lookup(s));
                }
            }
        }
    }

    /// Verifies one piece of a parse string, and remembers it if valid.
    /// All errors are not emitted as fatal so we can continue giving errors
    /// about this and possibly other format strings.
    fn verify_piece(&mut self, p: &parse::Piece<'_>) {
        match *p {
            parse::String(..) => {}
            parse::NextArgument(ref arg) => {
                // width/precision first, if they have implicit positional
                // parameters it makes more sense to consume them first.
                self.verify_count(arg.format.width);
                self.verify_count(arg.format.precision);

                // argument second, if it's an implicit positional parameter
                // it's written second, so it should come after width/precision.
                let pos = match arg.position {
                    parse::ArgumentIs(i) | parse::ArgumentImplicitlyIs(i) => Exact(i),
                    parse::ArgumentNamed(s) => Named(s),
                };

                let ty = Placeholder(arg.format.ty.to_string());
                self.verify_arg_type(pos, ty);
                self.curpiece += 1;
            }
        }
    }

    fn verify_count(&mut self, c: parse::Count) {
        match c {
            parse::CountImplied |
            parse::CountIs(..) => {}
            parse::CountIsParam(i) => {
                self.verify_arg_type(Exact(i), Count);
            }
            parse::CountIsName(s) => {
                self.verify_arg_type(Named(s), Count);
            }
        }
    }

    fn describe_num_args(&self) -> Cow<'_, str> {
        match self.args.len() {
            0 => "no arguments were given".into(),
            1 => "there is 1 argument".into(),
            x => format!("there are {} arguments", x).into(),
        }
    }

    /// Handle invalid references to positional arguments. Output different
    /// errors for the case where all arguments are positional and for when
    /// there are named arguments or numbered positional arguments in the
    /// format string.
    fn report_invalid_references(&self, numbered_position_args: bool) {
        let mut e;
        let sp = if self.is_literal {
            MultiSpan::from_spans(self.arg_spans.clone())
        } else {
            MultiSpan::from_span(self.fmtsp)
        };
        let refs_len = self.invalid_refs.len();
        let mut refs = self
            .invalid_refs
            .iter()
            .map(|(r, pos)| (r.to_string(), self.arg_spans.get(*pos)));

        if self.names.is_empty() && !numbered_position_args {
            e = self.ecx.mut_span_err(
                sp,
                &format!(
                    "{} positional argument{} in format string, but {}",
                         self.pieces.len(),
                         if self.pieces.len() > 1 { "s" } else { "" },
                    self.describe_num_args()
                ),
            );
        } else {
            let (arg_list, mut sp) = if refs_len == 1 {
                let (reg, pos) = refs.next().unwrap();
                (
                    format!("argument {}", reg),
                    MultiSpan::from_span(*pos.unwrap_or(&self.fmtsp)),
                )
            } else {
                let (mut refs, spans): (Vec<_>, Vec<_>) = refs.unzip();
                let pos = MultiSpan::from_spans(spans.into_iter().map(|s| *s.unwrap()).collect());
                let reg = refs.pop().unwrap();
                (
                    format!(
                        "arguments {head} and {tail}",
                        head = refs.join(", "),
                        tail = reg,
                    ),
                    pos,
                )
            };
            if !self.is_literal {
                sp = MultiSpan::from_span(self.fmtsp);
            }

            e = self.ecx.mut_span_err(sp,
                &format!("invalid reference to positional {} ({})",
                         arg_list,
                         self.describe_num_args()));
            e.note("positional arguments are zero-based");
        };

        e.emit();
    }

    /// Actually verifies and tracks a given format placeholder
    /// (a.k.a. argument).
    fn verify_arg_type(&mut self, arg: Position, ty: ArgumentType) {
        match arg {
            Exact(arg) => {
                if self.args.len() <= arg {
                    self.invalid_refs.push((arg, self.curpiece));
                    return;
                }
                match ty {
                    Placeholder(_) => {
                        // record every (position, type) combination only once
                        let ref mut seen_ty = self.arg_unique_types[arg];
                        let i = seen_ty.iter().position(|x| *x == ty).unwrap_or_else(|| {
                            let i = seen_ty.len();
                            seen_ty.push(ty);
                            i
                        });
                        self.arg_types[arg].push(i);
                    }
                    Count => {
                        if let Entry::Vacant(e) = self.count_positions.entry(arg) {
                            let i = self.count_positions_count;
                            e.insert(i);
                            self.count_args.push(Exact(arg));
                            self.count_positions_count += 1;
                        }
                    }
                }
            }

            Named(name) => {
                match self.names.get(&name) {
                    Some(&idx) => {
                        // Treat as positional arg.
                        self.verify_arg_type(Exact(idx), ty)
                    }
                    None => {
                        let msg = format!("there is no argument named `{}`", name);
                        let sp = if self.is_literal {
                            *self.arg_spans.get(self.curpiece).unwrap_or(&self.fmtsp)
                        } else {
                            self.fmtsp
                        };
                        let mut err = self.ecx.struct_span_err(sp, &msg[..]);
                        err.emit();
                    }
                }
            }
        }
    }

    /// Builds the mapping between format placeholders and argument objects.
    fn build_index_map(&mut self) {
        // NOTE: Keep the ordering the same as `into_expr`'s expansion would do!
        let args_len = self.args.len();
        self.arg_index_map.reserve(args_len);

        let mut sofar = 0usize;

        // Map the arguments
        for i in 0..args_len {
            let ref arg_types = self.arg_types[i];
            let arg_offsets = arg_types.iter().map(|offset| sofar + *offset).collect::<Vec<_>>();
            self.arg_index_map.push(arg_offsets);
            sofar += self.arg_unique_types[i].len();
        }

        // Record starting index for counts, which appear just after arguments
        self.count_args_index_offset = sofar;
    }

    fn rtpath(ecx: &ExtCtxt<'_>, s: &str) -> Vec<ast::Ident> {
        ecx.std_path(&[sym::fmt, sym::rt, sym::v1, Symbol::intern(s)])
    }

    fn build_count(&self, c: parse::Count) -> P<ast::Expr> {
        let sp = self.macsp;
        let count = |c, arg| {
            let mut path = Context::rtpath(self.ecx, "Count");
            path.push(self.ecx.ident_of(c));
            match arg {
                Some(arg) => self.ecx.expr_call_global(sp, path, vec![arg]),
                None => self.ecx.expr_path(self.ecx.path_global(sp, path)),
            }
        };
        match c {
            parse::CountIs(i) => count("Is", Some(self.ecx.expr_usize(sp, i))),
            parse::CountIsParam(i) => {
                // This needs mapping too, as `i` is referring to a macro
                // argument. If `i` is not found in `count_positions` then
                // the error had already been emitted elsewhere.
                let i = self.count_positions.get(&i).cloned().unwrap_or(0)
                      + self.count_args_index_offset;
                count("Param", Some(self.ecx.expr_usize(sp, i)))
            }
            parse::CountImplied => count("Implied", None),
            // should never be the case, names are already resolved
            parse::CountIsName(_) => panic!("should never happen"),
        }
    }

    /// Build a literal expression from the accumulated string literals
    fn build_literal_string(&mut self) -> P<ast::Expr> {
        let sp = self.fmtsp;
        let s = Symbol::intern(&self.literal);
        self.literal.clear();
        self.ecx.expr_str(sp, s)
    }

    /// Builds a static `rt::Argument` from a `parse::Piece` or append
    /// to the `literal` string.
    fn build_piece(&mut self,
                   piece: &parse::Piece<'_>,
                   arg_index_consumed: &mut Vec<usize>)
                   -> Option<P<ast::Expr>> {
        let sp = self.macsp;
        match *piece {
            parse::String(s) => {
                self.literal.push_str(s);
                None
            }
            parse::NextArgument(ref arg) => {
                // Build the position
                let pos = {
                    let pos = |c, arg| {
                        let mut path = Context::rtpath(self.ecx, "Position");
                        path.push(self.ecx.ident_of(c));
                        match arg {
                            Some(i) => {
                                let arg = self.ecx.expr_usize(sp, i);
                                self.ecx.expr_call_global(sp, path, vec![arg])
                            }
                            None => self.ecx.expr_path(self.ecx.path_global(sp, path)),
                        }
                    };
                    match arg.position {
                        parse::ArgumentIs(i)
                        | parse::ArgumentImplicitlyIs(i) => {
                            // Map to index in final generated argument array
                            // in case of multiple types specified
                            let arg_idx = match arg_index_consumed.get_mut(i) {
                                None => 0, // error already emitted elsewhere
                                Some(offset) => {
                                    let ref idx_map = self.arg_index_map[i];
                                    // unwrap_or branch: error already emitted elsewhere
                                    let arg_idx = *idx_map.get(*offset).unwrap_or(&0);
                                    *offset += 1;
                                    arg_idx
                                }
                            };
                            pos("At", Some(arg_idx))
                        }

                        // should never be the case, because names are already
                        // resolved.
                        parse::ArgumentNamed(_) => panic!("should never happen"),
                    }
                };

                let simple_arg = parse::Argument {
                    position: {
                        // We don't have ArgumentNext any more, so we have to
                        // track the current argument ourselves.
                        let i = self.curarg;
                        self.curarg += 1;
                        parse::ArgumentIs(i)
                    },
                    format: parse::FormatSpec {
                        fill: arg.format.fill,
                        align: parse::AlignUnknown,
                        flags: 0,
                        precision: parse::CountImplied,
                        width: parse::CountImplied,
                        ty: arg.format.ty,
                    },
                };

                let fill = arg.format.fill.unwrap_or(' ');

                let pos_simple =
                    arg.position.index() == simple_arg.position.index();

                if !pos_simple || arg.format != simple_arg.format || fill != ' ' {
                    self.all_pieces_simple = false;
                }

                // Build the format
                let fill = self.ecx.expr_lit(sp, ast::LitKind::Char(fill));
                let align = |name| {
                    let mut p = Context::rtpath(self.ecx, "Alignment");
                    p.push(self.ecx.ident_of(name));
                    self.ecx.path_global(sp, p)
                };
                let align = match arg.format.align {
                    parse::AlignLeft => align("Left"),
                    parse::AlignRight => align("Right"),
                    parse::AlignCenter => align("Center"),
                    parse::AlignUnknown => align("Unknown"),
                };
                let align = self.ecx.expr_path(align);
                let flags = self.ecx.expr_u32(sp, arg.format.flags);
                let prec = self.build_count(arg.format.precision);
                let width = self.build_count(arg.format.width);
                let path = self.ecx.path_global(sp, Context::rtpath(self.ecx, "FormatSpec"));
                let fmt = self.ecx.expr_struct(
                    sp,
                                         path,
                    vec![
                        self.ecx.field_imm(sp, self.ecx.ident_of("fill"), fill),
                        self.ecx.field_imm(sp, self.ecx.ident_of("align"), align),
                        self.ecx.field_imm(sp, self.ecx.ident_of("flags"), flags),
                        self.ecx.field_imm(sp, self.ecx.ident_of("precision"), prec),
                        self.ecx.field_imm(sp, self.ecx.ident_of("width"), width),
                    ],
                );

                let path = self.ecx.path_global(sp, Context::rtpath(self.ecx, "Argument"));
                Some(self.ecx.expr_struct(
                    sp,
                                          path,
                    vec![
                        self.ecx.field_imm(sp, self.ecx.ident_of("position"), pos),
                        self.ecx.field_imm(sp, self.ecx.ident_of("format"), fmt),
                    ],
                ))
            }
        }
    }

    /// Actually builds the expression which the format_args! block will be
    /// expanded to.
    fn into_expr(self) -> P<ast::Expr> {
        let mut locals = Vec::with_capacity(
            (0..self.args.len()).map(|i| self.arg_unique_types[i].len()).sum()
        );
        let mut counts = Vec::with_capacity(self.count_args.len());
        let mut pats = Vec::with_capacity(self.args.len());
        let mut heads = Vec::with_capacity(self.args.len());

        let names_pos: Vec<_> = (0..self.args.len())
            .map(|i| self.ecx.ident_of(&format!("arg{}", i)).gensym())
            .collect();

        // First, build up the static array which will become our precompiled
        // format "string"
        let pieces = self.ecx.expr_vec_slice(self.fmtsp, self.str_pieces);

        // Before consuming the expressions, we have to remember spans for
        // count arguments as they are now generated separate from other
        // arguments, hence have no access to the `P<ast::Expr>`'s.
        let spans_pos: Vec<_> = self.args.iter().map(|e| e.span.clone()).collect();

        // Right now there is a bug such that for the expression:
        //      foo(bar(&1))
        // the lifetime of `1` doesn't outlast the call to `bar`, so it's not
        // valid for the call to `foo`. To work around this all arguments to the
        // format! string are shoved into locals. Furthermore, we shove the address
        // of each variable because we don't want to move out of the arguments
        // passed to this function.
        for (i, e) in self.args.into_iter().enumerate() {
            let name = names_pos[i];
            let span =
                DUMMY_SP.with_ctxt(e.span.ctxt().apply_mark(self.ecx.current_expansion.mark));
            pats.push(self.ecx.pat_ident(span, name));
            for ref arg_ty in self.arg_unique_types[i].iter() {
                locals.push(Context::format_arg(self.ecx, self.macsp, e.span, arg_ty, name));
            }
            heads.push(self.ecx.expr_addr_of(e.span, e));
        }
        for pos in self.count_args {
            let index = match pos {
                Exact(i) => i,
                _ => panic!("should never happen"),
            };
            let name = names_pos[index];
            let span = spans_pos[index];
            counts.push(Context::format_arg(self.ecx, self.macsp, span, &Count, name));
        }

        // Now create a vector containing all the arguments
        let args = locals.into_iter().chain(counts.into_iter());

        let args_array = self.ecx.expr_vec(self.fmtsp, args.collect());

        // Constructs an AST equivalent to:
        //
        //      match (&arg0, &arg1) {
        //          (tmp0, tmp1) => args_array
        //      }
        //
        // It was:
        //
        //      let tmp0 = &arg0;
        //      let tmp1 = &arg1;
        //      args_array
        //
        // Because of #11585 the new temporary lifetime rule, the enclosing
        // statements for these temporaries become the let's themselves.
        // If one or more of them are RefCell's, RefCell borrow() will also
        // end there; they don't last long enough for args_array to use them.
        // The match expression solves the scope problem.
        //
        // Note, it may also very well be transformed to:
        //
        //      match arg0 {
        //          ref tmp0 => {
        //              match arg1 => {
        //                  ref tmp1 => args_array } } }
        //
        // But the nested match expression is proved to perform not as well
        // as series of let's; the first approach does.
        let pat = self.ecx.pat_tuple(self.fmtsp, pats);
        let arm = self.ecx.arm(self.fmtsp, vec![pat], args_array);
        let head = self.ecx.expr(self.fmtsp, ast::ExprKind::Tup(heads));
        let result = self.ecx.expr_match(self.fmtsp, head, vec![arm]);

        let args_slice = self.ecx.expr_addr_of(self.fmtsp, result);

        // Now create the fmt::Arguments struct with all our locals we created.
        let (fn_name, fn_args) = if self.all_pieces_simple {
            ("new_v1", vec![pieces, args_slice])
        } else {
            // Build up the static array which will store our precompiled
            // nonstandard placeholders, if there are any.
            let fmt = self.ecx.expr_vec_slice(self.macsp, self.pieces);

            ("new_v1_formatted", vec![pieces, args_slice, fmt])
        };

        let path = self.ecx.std_path(&[sym::fmt, sym::Arguments, Symbol::intern(fn_name)]);
        self.ecx.expr_call_global(self.macsp, path, fn_args)
    }

    fn format_arg(ecx: &ExtCtxt<'_>,
                  macsp: Span,
                  mut sp: Span,
                  ty: &ArgumentType,
                  arg: ast::Ident)
                  -> P<ast::Expr> {
        sp = sp.apply_mark(ecx.current_expansion.mark);
        let arg = ecx.expr_ident(sp, arg);
        let trait_ = match *ty {
            Placeholder(ref tyname) => {
                match &tyname[..] {
                    "" => "Display",
                    "?" => "Debug",
                    "e" => "LowerExp",
                    "E" => "UpperExp",
                    "o" => "Octal",
                    "p" => "Pointer",
                    "b" => "Binary",
                    "x" => "LowerHex",
                    "X" => "UpperHex",
                    _ => {
                        ecx.span_err(sp, &format!("unknown format trait `{}`", *tyname));
                        return DummyResult::raw_expr(sp, true);
                    }
                }
            }
            Count => {
                let path = ecx.std_path(&[sym::fmt, sym::ArgumentV1, sym::from_usize]);
                return ecx.expr_call_global(macsp, path, vec![arg]);
            }
        };

        let path = ecx.std_path(&[sym::fmt, Symbol::intern(trait_), sym::fmt]);
        let format_fn = ecx.path_global(sp, path);
        let path = ecx.std_path(&[sym::fmt, sym::ArgumentV1, sym::new]);
        ecx.expr_call_global(macsp, path, vec![arg, ecx.expr_path(format_fn)])
    }
}

fn expand_format_args_impl<'cx>(
    ecx: &'cx mut ExtCtxt<'_>,
    mut sp: Span,
    tts: &[tokenstream::TokenTree],
    nl: bool,
) -> Box<dyn base::MacResult + 'cx> {
    sp = sp.apply_mark(ecx.current_expansion.mark);
    match parse_args(ecx, sp, tts) {
        Ok((efmt, args, names)) => {
            MacEager::expr(expand_preparsed_format_args(ecx, sp, efmt, args, names, nl))
        }
        Err(mut err) => {
            err.emit();
            DummyResult::expr(sp)
        }
    }
}

pub fn expand_format_args<'cx>(
    ecx: &'cx mut ExtCtxt<'_>,
    sp: Span,
    tts: &[tokenstream::TokenTree],
) -> Box<dyn base::MacResult + 'cx> {
    expand_format_args_impl(ecx, sp, tts, false)
}

pub fn expand_format_args_nl<'cx>(
    ecx: &'cx mut ExtCtxt<'_>,
    sp: Span,
    tts: &[tokenstream::TokenTree],
) -> Box<dyn base::MacResult + 'cx> {
    expand_format_args_impl(ecx, sp, tts, true)
}

/// Take the various parts of `format_args!(efmt, args..., name=names...)`
/// and construct the appropriate formatting expression.
pub fn expand_preparsed_format_args(ecx: &mut ExtCtxt<'_>,
                                    sp: Span,
                                    efmt: P<ast::Expr>,
                                    args: Vec<P<ast::Expr>>,
                                    names: FxHashMap<Symbol, usize>,
                                    append_newline: bool)
                                    -> P<ast::Expr> {
    // NOTE: this verbose way of initializing `Vec<Vec<ArgumentType>>` is because
    // `ArgumentType` does not derive `Clone`.
    let arg_types: Vec<_> = (0..args.len()).map(|_| Vec::new()).collect();
    let arg_unique_types: Vec<_> = (0..args.len()).map(|_| Vec::new()).collect();

    let mut macsp = ecx.call_site();
    macsp = macsp.apply_mark(ecx.current_expansion.mark);

    let msg = "format argument must be a string literal";
    let fmt_sp = efmt.span;
    let fmt = match expr_to_spanned_string(ecx, efmt, msg) {
        Ok(mut fmt) if append_newline => {
            fmt.node.0 = Symbol::intern(&format!("{}\n", fmt.node.0));
            fmt
        }
        Ok(fmt) => fmt,
        Err(err) => {
            if let Some(mut err) = err {
                let sugg_fmt = match args.len() {
                    0 => "{}".to_string(),
                    _ => format!("{}{{}}", "{} ".repeat(args.len())),
                };
                err.span_suggestion(
                    fmt_sp.shrink_to_lo(),
                    "you might be missing a string literal to format with",
                    format!("\"{}\", ", sugg_fmt),
                    Applicability::MaybeIncorrect,
                );
                err.emit();
            }
            return DummyResult::raw_expr(sp, true);
        }
    };

    let (is_literal, fmt_snippet) = match ecx.source_map().span_to_snippet(fmt_sp) {
        Ok(s) => (s.starts_with("\"") || s.starts_with("r#"), Some(s)),
        _ => (false, None),
    };

    let str_style = match fmt.node.1 {
        ast::StrStyle::Cooked => None,
        ast::StrStyle::Raw(raw) => {
            Some(raw as usize)
        },
    };

    /// Finds the indices of all characters that have been processed and differ between the actual
    /// written code (code snippet) and the `InternedString` that get's processed in the `Parser`
    /// in order to properly synthethise the intra-string `Span`s for error diagnostics.
    fn find_skips(snippet: &str, is_raw: bool) -> Vec<usize> {
        let mut eat_ws = false;
        let mut s = snippet.chars().enumerate().peekable();
        let mut skips = vec![];
        while let Some((pos, c)) = s.next() {
            match (c, s.peek()) {
                // skip whitespace and empty lines ending in '\\'
                ('\\', Some((next_pos, '\n'))) if !is_raw => {
                    eat_ws = true;
                    skips.push(pos);
                    skips.push(*next_pos);
                    let _ = s.next();
                }
                ('\\', Some((next_pos, '\n'))) |
                ('\\', Some((next_pos, 'n'))) |
                ('\\', Some((next_pos, 't'))) if eat_ws => {
                    skips.push(pos);
                    skips.push(*next_pos);
                    let _ = s.next();
                }
                (' ', _) |
                ('\n', _) |
                ('\t', _) if eat_ws => {
                    skips.push(pos);
                }
                ('\\', Some((next_pos, 'n'))) |
                ('\\', Some((next_pos, 't'))) |
                ('\\', Some((next_pos, '0'))) |
                ('\\', Some((next_pos, '\\'))) |
                ('\\', Some((next_pos, '\''))) |
                ('\\', Some((next_pos, '\"'))) => {
                    skips.push(*next_pos);
                    let _ = s.next();
                }
                ('\\', Some((_, 'x'))) if !is_raw => {
                    for _ in 0..3 {  // consume `\xAB` literal
                        if let Some((pos, _)) = s.next() {
                            skips.push(pos);
                        } else {
                            break;
                        }
                    }
                }
                ('\\', Some((_, 'u'))) if !is_raw => {
                    if let Some((pos, _)) = s.next() {
                        skips.push(pos);
                    }
                    if let Some((next_pos, next_c)) = s.next() {
                        if next_c == '{' {
                            skips.push(next_pos);
                            let mut i = 0;  // consume up to 6 hexanumeric chars + closing `}`
                            while let (Some((next_pos, c)), true) = (s.next(), i < 7) {
                                if c.is_digit(16) {
                                    skips.push(next_pos);
                                } else if c == '}' {
                                    skips.push(next_pos);
                                    break;
                                } else {
                                    break;
                                }
                                i += 1;
                            }
                        } else if next_c.is_digit(16) {
                            skips.push(next_pos);
                            // We suggest adding `{` and `}` when appropriate, accept it here as if
                            // it were correct
                            let mut i = 0;  // consume up to 6 hexanumeric chars
                            while let (Some((next_pos, c)), _) = (s.next(), i < 6) {
                                if c.is_digit(16) {
                                    skips.push(next_pos);
                                } else {
                                    break;
                                }
                                i += 1;
                            }
                        }
                    }
                }
                _ if eat_ws => {  // `take_while(|c| c.is_whitespace())`
                    eat_ws = false;
                }
                _ => {}
            }
        }
        skips
    }

    let skips = if let (true, Some(ref snippet)) = (is_literal, fmt_snippet.as_ref()) {
        let r_start = str_style.map(|r| r + 1).unwrap_or(0);
        let r_end = str_style.map(|r| r).unwrap_or(0);
        let s = &snippet[r_start + 1..snippet.len() - r_end - 1];
        find_skips(s, str_style.is_some())
    } else {
        vec![]
    };

    let fmt_str = &*fmt.node.0.as_str();  // for the suggestions below
    let mut parser = parse::Parser::new(fmt_str, str_style, skips, append_newline);

    let mut unverified_pieces = Vec::new();
    while let Some(piece) = parser.next() {
        if !parser.errors.is_empty() {
            break;
        } else {
            unverified_pieces.push(piece);
        }
    }

    if !parser.errors.is_empty() {
        let err = parser.errors.remove(0);
        let sp = fmt.span.from_inner(err.span);
        let mut e = ecx.struct_span_err(sp, &format!("invalid format string: {}",
                                                     err.description));
        e.span_label(sp, err.label + " in format string");
        if let Some(note) = err.note {
            e.note(&note);
        }
        if let Some((label, span)) = err.secondary_label {
            let sp = fmt.span.from_inner(span);
            e.span_label(sp, label);
        }
        e.emit();
        return DummyResult::raw_expr(sp, true);
    }

    let arg_spans = parser.arg_places.iter()
        .map(|span| fmt.span.from_inner(*span))
        .collect();

    let mut cx = Context {
        ecx,
        args,
        arg_types,
        arg_unique_types,
        names,
        curarg: 0,
        curpiece: 0,
        arg_index_map: Vec::new(),
        count_args: Vec::new(),
        count_positions: FxHashMap::default(),
        count_positions_count: 0,
        count_args_index_offset: 0,
        literal: String::new(),
        pieces: Vec::with_capacity(unverified_pieces.len()),
        str_pieces: Vec::with_capacity(unverified_pieces.len()),
        all_pieces_simple: true,
        macsp,
        fmtsp: fmt.span,
        invalid_refs: Vec::new(),
        arg_spans,
        is_literal,
    };

    // This needs to happen *after* the Parser has consumed all pieces to create all the spans
    let pieces = unverified_pieces.into_iter().map(|mut piece| {
        cx.verify_piece(&piece);
        cx.resolve_name_inplace(&mut piece);
        piece
    }).collect::<Vec<_>>();

    let numbered_position_args = pieces.iter().any(|arg: &parse::Piece<'_>| {
        match *arg {
            parse::String(_) => false,
            parse::NextArgument(arg) => {
                match arg.position {
                    parse::Position::ArgumentIs(_) => true,
                    _ => false,
                }
            }
        }
    });

    cx.build_index_map();

    let mut arg_index_consumed = vec![0usize; cx.arg_index_map.len()];

    for piece in pieces {
        if let Some(piece) = cx.build_piece(&piece, &mut arg_index_consumed) {
            let s = cx.build_literal_string();
            cx.str_pieces.push(s);
            cx.pieces.push(piece);
        }
    }

    if !cx.literal.is_empty() {
        let s = cx.build_literal_string();
        cx.str_pieces.push(s);
    }

    if cx.invalid_refs.len() >= 1 {
        cx.report_invalid_references(numbered_position_args);
    }

    // Make sure that all arguments were used and all arguments have types.
    let num_pos_args = cx.args.len() - cx.names.len();

    let errs = cx.arg_types
                 .iter()
                 .enumerate()
                 .filter(|(i, ty)| ty.is_empty() && !cx.count_positions.contains_key(&i))
                 .map(|(i, _)| {
                    let msg = if i >= num_pos_args {
                        // named argument
                        "named argument never used"
                    } else {
                        // positional argument
                        "argument never used"
                    };
                    (cx.args[i].span, msg)
                 })
                 .collect::<Vec<_>>();

    let errs_len = errs.len();
    if !errs.is_empty() {
        let args_used = cx.arg_types.len() - errs_len;
        let args_unused = errs_len;

        let mut diag = {
            if errs_len == 1 {
                let (sp, msg) = errs.into_iter().next().unwrap();
                let mut diag = cx.ecx.struct_span_err(sp, msg);
                diag.span_label(sp, msg);
                diag
            } else {
                let mut diag = cx.ecx.struct_span_err(
                    errs.iter().map(|&(sp, _)| sp).collect::<Vec<Span>>(),
                    "multiple unused formatting arguments",
                );
                diag.span_label(cx.fmtsp, "multiple missing formatting specifiers");
                for (sp, msg) in errs {
                    diag.span_label(sp, msg);
                }
                diag
            }
        };

        // Used to ensure we only report translations for *one* kind of foreign format.
        let mut found_foreign = false;
        // Decide if we want to look for foreign formatting directives.
        if args_used < args_unused {
            use super::format_foreign as foreign;

            // The set of foreign substitutions we've explained.  This prevents spamming the user
            // with `%d should be written as {}` over and over again.
            let mut explained = FxHashSet::default();

            macro_rules! check_foreign {
                ($kind:ident) => {{
                    let mut show_doc_note = false;

                    let mut suggestions = vec![];
                    // account for `"` and account for raw strings `r#`
                    let padding = str_style.map(|i| i + 2).unwrap_or(1);
                    for sub in foreign::$kind::iter_subs(fmt_str, padding) {
                        let trn = match sub.translate() {
                            Some(trn) => trn,

                            // If it has no translation, don't call it out specifically.
                            None => continue,
                        };

                        let pos = sub.position();
                        let sub = String::from(sub.as_str());
                        if explained.contains(&sub) {
                            continue;
                        }
                        explained.insert(sub.clone());

                        if !found_foreign {
                            found_foreign = true;
                            show_doc_note = true;
                        }

                        if let Some(inner_sp) = pos {
                            let sp = fmt_sp.from_inner(inner_sp);
                            suggestions.push((sp, trn));
                        } else {
                            diag.help(&format!("`{}` should be written as `{}`", sub, trn));
                        }
                    }

                    if show_doc_note {
                        diag.note(concat!(
                            stringify!($kind),
                            " formatting not supported; see the documentation for `std::fmt`",
                        ));
                    }
                    if suggestions.len() > 0 {
                        diag.multipart_suggestion(
                            "format specifiers use curly braces",
                            suggestions,
                            Applicability::MachineApplicable,
                        );
                    }
                }};
            }

            check_foreign!(printf);
            if !found_foreign {
                check_foreign!(shell);
            }
        }
        if !found_foreign && errs_len == 1 {
            diag.span_label(cx.fmtsp, "formatting specifier missing");
        }

        diag.emit();
    }

    cx.into_expr()
}
