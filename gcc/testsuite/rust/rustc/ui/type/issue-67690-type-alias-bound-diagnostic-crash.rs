// Regression test for issue #67690
// Rustc endless loop out-of-memory and consequent SIGKILL in generic new type

// check-pass
pub type T<P: Send + Send + Send> = P;
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {}

