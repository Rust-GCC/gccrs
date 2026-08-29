#![feature(use_extern_macros)]
trait Foo {}
#[derive(Foo::Anything)] // { dg-error ".E0433." "" { target *-*-* } }
// { dg-error ".E0433." "" { target *-*-* } .-2 }
struct S;

fn main() {}

