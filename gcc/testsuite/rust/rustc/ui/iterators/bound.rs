struct S<I: Iterator>(I);
struct T(S<u8>);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
fn main() {}

