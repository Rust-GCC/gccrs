#[derive(::Absolute)] // { dg-error ".E0433." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
struct S;

fn main() {}

