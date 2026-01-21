// aux-build:test-macros.rs

#[macro_use]
extern crate test_macros;

#[derive_Empty] // { dg-error "" "" { target *-*-* } }
struct A;

fn main() {}

