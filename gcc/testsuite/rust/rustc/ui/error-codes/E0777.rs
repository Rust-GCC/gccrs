#[derive("Clone")] // { dg-error ".E0777." "" { target *-*-* } }
#[derive("Clone
")]
// { dg-error ".E0777." "" { target *-*-* } .-2 }
struct Foo;

fn main() {}

