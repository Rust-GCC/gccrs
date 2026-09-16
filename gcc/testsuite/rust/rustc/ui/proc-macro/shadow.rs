// aux-build:test-macros.rs

#[macro_use]
extern crate test_macros;
#[macro_use]
extern crate test_macros; // { dg-error ".E0259." "" { target *-*-* } }

fn main() {}

