// Issue 69130: character indexing bug in rustc_errors::CodeSuggestion::splice_lines().

enum F {
M (§& u8)}
// { dg-error ".E0106." "" { target *-*-* } .-1 }
// { dg-error ".E0106." "" { target *-*-* } .-2 }
fn main() {}

