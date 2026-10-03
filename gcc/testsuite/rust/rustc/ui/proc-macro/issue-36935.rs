// aux-build:test-macros.rs

#[macro_use]
extern crate test_macros;

#[derive(Identity, Panic)] // { dg-error "" "" { target *-*-* } }
struct Baz {
// { dg-error ".E0428." "" { target *-*-* } .-1 }
    a: i32,
    b: i32,
}

fn main() {}

