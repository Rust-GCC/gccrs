// aux-build:test-macros.rs

extern crate test_macros;

use test_macros::empty_derive;
// { dg-error ".E0432." "" { target *-*-* } .-1 }

fn main() {}

