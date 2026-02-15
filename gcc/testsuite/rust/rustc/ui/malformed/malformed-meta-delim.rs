fn main() {}

#[allow { foo_lint } ]
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
fn delim_brace() {}

#[allow [ foo_lint ] ]
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
fn delim_bracket() {}

