// aux-build:m1.rs
// aux-build:m2.rs


extern crate m1;
extern crate m2 as m1; // { dg-error ".E0259." "" { target *-*-* } }

fn main() {}

