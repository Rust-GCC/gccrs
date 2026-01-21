// This test needs to the last one appearing in this file as it kills the parser
static c: char =
    'nope' // { dg-error "" "" { target *-*-* } }
;

fn main() {}

