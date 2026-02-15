// Regression test for issue #4935

fn foo(a: usize) {}
// { dg-error "" "" { target *-*-* } .-1 }
fn main() { foo(5, 6) }
// { dg-error ".E0061." "" { target *-*-* } .-1 }

