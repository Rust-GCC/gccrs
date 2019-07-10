pub use CommentStyle::*;

use crate::ast;
use crate::source_map::SourceMap;
use crate::parse::lexer::{is_block_doc_comment, is_pattern_whitespace};
use crate::parse::lexer::{self, ParseSess, StringReader};

use syntax_pos::{BytePos, CharPos, Pos, FileName};
use log::debug;

use std::io::Read;
use std::usize;

#[derive(Clone, Copy, PartialEq, Debug)]
pub enum CommentStyle {
    /// No code on either side of each line of the comment
    Isolated,
    /// Code exists to the left of the comment
    Trailing,
    /// Code before /* foo */ and after the comment
    Mixed,
    /// Just a manual blank line "\n\n", for layout
    BlankLine,
}

#[derive(Clone)]
pub struct Comment {
    pub style: CommentStyle,
    pub lines: Vec<String>,
    pub pos: BytePos,
}

fn is_doc_comment(s: &str) -> bool {
    (s.starts_with("///") && super::is_doc_comment(s)) || s.starts_with("//!") ||
    (s.starts_with("/**") && is_block_doc_comment(s)) || s.starts_with("/*!")
}

pub fn doc_comment_style(comment: &str) -> ast::AttrStyle {
    assert!(is_doc_comment(comment));
    if comment.starts_with("//!") || comment.starts_with("/*!") {
        ast::AttrStyle::Inner
    } else {
        ast::AttrStyle::Outer
    }
}

pub fn strip_doc_comment_decoration(comment: &str) -> String {
    /// remove whitespace-only lines from the start/end of lines
    fn vertical_trim(lines: Vec<String>) -> Vec<String> {
        let mut i = 0;
        let mut j = lines.len();
        // first line of all-stars should be omitted
        if !lines.is_empty() && lines[0].chars().all(|c| c == '*') {
            i += 1;
        }

        while i < j && lines[i].trim().is_empty() {
            i += 1;
        }
        // like the first, a last line of all stars should be omitted
        if j > i &&
           lines[j - 1]
               .chars()
               .skip(1)
               .all(|c| c == '*') {
            j -= 1;
        }

        while j > i && lines[j - 1].trim().is_empty() {
            j -= 1;
        }

        lines[i..j].to_vec()
    }

    /// remove a "[ \t]*\*" block from each line, if possible
    fn horizontal_trim(lines: Vec<String>) -> Vec<String> {
        let mut i = usize::MAX;
        let mut can_trim = true;
        let mut first = true;

        for line in &lines {
            for (j, c) in line.chars().enumerate() {
                if j > i || !"* \t".contains(c) {
                    can_trim = false;
                    break;
                }
                if c == '*' {
                    if first {
                        i = j;
                        first = false;
                    } else if i != j {
                        can_trim = false;
                    }
                    break;
                }
            }
            if i >= line.len() {
                can_trim = false;
            }
            if !can_trim {
                break;
            }
        }

        if can_trim {
            lines.iter()
                 .map(|line| (&line[i + 1..line.len()]).to_string())
                 .collect()
        } else {
            lines
        }
    }

    // one-line comments lose their prefix
    const ONELINERS: &[&str] = &["///!", "///", "//!", "//"];

    for prefix in ONELINERS {
        if comment.starts_with(*prefix) {
            return (&comment[prefix.len()..]).to_string();
        }
    }

    if comment.starts_with("/*") {
        let lines = comment[3..comment.len() - 2]
                        .lines()
                        .map(|s| s.to_string())
                        .collect::<Vec<String>>();

        let lines = vertical_trim(lines);
        let lines = horizontal_trim(lines);

        return lines.join("\n");
    }

    panic!("not a doc-comment: {}", comment);
}

fn push_blank_line_comment(rdr: &StringReader<'_>, comments: &mut Vec<Comment>) {
    debug!(">>> blank-line comment");
    comments.push(Comment {
        style: BlankLine,
        lines: Vec::new(),
        pos: rdr.pos,
    });
}

fn consume_whitespace_counting_blank_lines(
    rdr: &mut StringReader<'_>,
    comments: &mut Vec<Comment>
) {
    while is_pattern_whitespace(rdr.ch) && !rdr.is_eof() {
        if rdr.ch_is('\n') {
            push_blank_line_comment(rdr, &mut *comments);
        }
        rdr.bump();
    }
}

fn read_shebang_comment(rdr: &mut StringReader<'_>,
                        code_to_the_left: bool,
                        comments: &mut Vec<Comment>) {
    debug!(">>> shebang comment");
    let p = rdr.pos;
    debug!("<<< shebang comment");
    comments.push(Comment {
        style: if code_to_the_left { Trailing } else { Isolated },
        lines: vec![rdr.read_one_line_comment()],
        pos: p,
    });
}

fn read_line_comments(rdr: &mut StringReader<'_>,
                      code_to_the_left: bool,
                      comments: &mut Vec<Comment>) {
    debug!(">>> line comments");
    let p = rdr.pos;
    let mut lines: Vec<String> = Vec::new();
    while rdr.ch_is('/') && rdr.nextch_is('/') {
        let line = rdr.read_one_line_comment();
        debug!("{}", line);
        // Doc comments are not put in comments.
        if is_doc_comment(&line[..]) {
            break;
        }
        lines.push(line);
        rdr.consume_non_eol_whitespace();
    }
    debug!("<<< line comments");
    if !lines.is_empty() {
        comments.push(Comment {
            style: if code_to_the_left { Trailing } else { Isolated },
            lines,
            pos: p,
        });
    }
}

/// Returns `None` if the first `col` chars of `s` contain a non-whitespace char.
/// Otherwise returns `Some(k)` where `k` is first char offset after that leading
/// whitespace. Note that `k` may be outside bounds of `s`.
fn all_whitespace(s: &str, col: CharPos) -> Option<usize> {
    let mut idx = 0;
    for (i, ch) in s.char_indices().take(col.to_usize()) {
        if !ch.is_whitespace() {
            return None;
        }
        idx = i + ch.len_utf8();
    }
    Some(idx)
}

fn trim_whitespace_prefix_and_push_line(lines: &mut Vec<String>, s: String, col: CharPos) {
    let len = s.len();
    let s1 = match all_whitespace(&s[..], col) {
        Some(col) => {
            if col < len {
                s[col..len].to_string()
            } else {
                String::new()
            }
        }
        None => s,
    };
    debug!("pushing line: {}", s1);
    lines.push(s1);
}

fn read_block_comment(rdr: &mut StringReader<'_>,
                      code_to_the_left: bool,
                      comments: &mut Vec<Comment>) {
    debug!(">>> block comment");
    let p = rdr.pos;
    let mut lines: Vec<String> = Vec::new();

    // Count the number of chars since the start of the line by rescanning.
    let src_index = rdr.src_index(rdr.source_file.line_begin_pos(rdr.pos));
    let end_src_index = rdr.src_index(rdr.pos);
    assert!(src_index <= end_src_index,
        "src_index={}, end_src_index={}, line_begin_pos={}",
        src_index, end_src_index, rdr.source_file.line_begin_pos(rdr.pos).to_u32());

    let col = CharPos(rdr.src[src_index..end_src_index].chars().count());

    rdr.bump();
    rdr.bump();

    let mut curr_line = String::from("/*");

    // doc-comments are not really comments, they are attributes
    if (rdr.ch_is('*') && !rdr.nextch_is('*')) || rdr.ch_is('!') {
        while !(rdr.ch_is('*') && rdr.nextch_is('/')) && !rdr.is_eof() {
            curr_line.push(rdr.ch.unwrap());
            rdr.bump();
        }
        if !rdr.is_eof() {
            curr_line.push_str("*/");
            rdr.bump();
            rdr.bump();
        }
        if is_block_doc_comment(&curr_line[..]) {
            return;
        }
        assert!(!curr_line.contains('\n'));
        lines.push(curr_line);
    } else {
        let mut level: isize = 1;
        while level > 0 {
            debug!("=== block comment level {}", level);
            if rdr.is_eof() {
                rdr.fatal_span_(rdr.pos, rdr.pos, "unterminated block comment").raise();
            }
            if rdr.ch_is('\n') {
                trim_whitespace_prefix_and_push_line(&mut lines, curr_line, col);
                curr_line = String::new();
                rdr.bump();
            } else {
                curr_line.push(rdr.ch.unwrap());
                if rdr.ch_is('/') && rdr.nextch_is('*') {
                    rdr.bump();
                    rdr.bump();
                    curr_line.push('*');
                    level += 1;
                } else {
                    if rdr.ch_is('*') && rdr.nextch_is('/') {
                        rdr.bump();
                        rdr.bump();
                        curr_line.push('/');
                        level -= 1;
                    } else {
                        rdr.bump();
                    }
                }
            }
        }
        if !curr_line.is_empty() {
            trim_whitespace_prefix_and_push_line(&mut lines, curr_line, col);
        }
    }

    let mut style = if code_to_the_left {
        Trailing
    } else {
        Isolated
    };
    rdr.consume_non_eol_whitespace();
    if !rdr.is_eof() && !rdr.ch_is('\n') && lines.len() == 1 {
        style = Mixed;
    }
    debug!("<<< block comment");
    comments.push(Comment {
        style,
        lines,
        pos: p,
    });
}


fn consume_comment(rdr: &mut StringReader<'_>,
                   comments: &mut Vec<Comment>,
                   code_to_the_left: &mut bool,
                   anything_to_the_left: &mut bool) {
    debug!(">>> consume comment");
    if rdr.ch_is('/') && rdr.nextch_is('/') {
        read_line_comments(rdr, *code_to_the_left, comments);
        *code_to_the_left = false;
        *anything_to_the_left = false;
    } else if rdr.ch_is('/') && rdr.nextch_is('*') {
        read_block_comment(rdr, *code_to_the_left, comments);
        *anything_to_the_left = true;
    } else if rdr.ch_is('#') && rdr.nextch_is('!') {
        read_shebang_comment(rdr, *code_to_the_left, comments);
        *code_to_the_left = false;
        *anything_to_the_left = false;
    } else {
        panic!();
    }
    debug!("<<< consume comment");
}

// it appears this function is called only from pprust... that's
// probably not a good thing.
pub fn gather_comments(sess: &ParseSess, path: FileName, srdr: &mut dyn Read) -> Vec<Comment>
{
    let mut src = String::new();
    srdr.read_to_string(&mut src).unwrap();
    let cm = SourceMap::new(sess.source_map().path_mapping().clone());
    let source_file = cm.new_source_file(path, src);
    let mut rdr = lexer::StringReader::new(sess, source_file, None);

    let mut comments: Vec<Comment> = Vec::new();
    let mut code_to_the_left = false; // Only code
    let mut anything_to_the_left = false; // Code or comments

    while !rdr.is_eof() {
        loop {
            // Eat all the whitespace and count blank lines.
            rdr.consume_non_eol_whitespace();
            if rdr.ch_is('\n') {
                if anything_to_the_left {
                    rdr.bump(); // The line is not blank, do not count.
                }
                consume_whitespace_counting_blank_lines(&mut rdr, &mut comments);
                code_to_the_left = false;
                anything_to_the_left = false;
            }
            // Eat one comment group
            if rdr.peeking_at_comment() {
                consume_comment(&mut rdr, &mut comments,
                                &mut code_to_the_left, &mut anything_to_the_left);
            } else {
                break
            }
        }

        rdr.next_token();
        code_to_the_left = true;
        anything_to_the_left = true;
    }

    comments
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_block_doc_comment_1() {
        let comment = "/**\n * Test \n **  Test\n *   Test\n*/";
        let stripped = strip_doc_comment_decoration(comment);
        assert_eq!(stripped, " Test \n*  Test\n   Test");
    }

    #[test]
    fn test_block_doc_comment_2() {
        let comment = "/**\n * Test\n *  Test\n*/";
        let stripped = strip_doc_comment_decoration(comment);
        assert_eq!(stripped, " Test\n  Test");
    }

    #[test]
    fn test_block_doc_comment_3() {
        let comment = "/**\n let a: *i32;\n *a = 5;\n*/";
        let stripped = strip_doc_comment_decoration(comment);
        assert_eq!(stripped, " let a: *i32;\n *a = 5;");
    }

    #[test]
    fn test_block_doc_comment_4() {
        let comment = "/*******************\n test\n *********************/";
        let stripped = strip_doc_comment_decoration(comment);
        assert_eq!(stripped, " test");
    }

    #[test]
    fn test_line_doc_comment() {
        let stripped = strip_doc_comment_decoration("/// test");
        assert_eq!(stripped, " test");
        let stripped = strip_doc_comment_decoration("///! test");
        assert_eq!(stripped, " test");
        let stripped = strip_doc_comment_decoration("// test");
        assert_eq!(stripped, " test");
        let stripped = strip_doc_comment_decoration("// test");
        assert_eq!(stripped, " test");
        let stripped = strip_doc_comment_decoration("///test");
        assert_eq!(stripped, "test");
        let stripped = strip_doc_comment_decoration("///!test");
        assert_eq!(stripped, "test");
        let stripped = strip_doc_comment_decoration("//test");
        assert_eq!(stripped, "test");
    }
}
