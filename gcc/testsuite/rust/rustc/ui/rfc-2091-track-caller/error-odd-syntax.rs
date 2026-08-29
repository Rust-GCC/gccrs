#[track_caller(1)]
fn f() {}
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

