// aux-build:test-macros.rs

#[macro_use]
extern crate test_macros;

#[derive(Panic)]
// { dg-error "" "" { target *-*-* } .-1 }
struct Foo;

fn main() {}

