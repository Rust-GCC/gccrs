// aux-build:m1.rs


extern crate m1;

struct X {
}

impl m1::X for X { // { dg-error ".E0046." "" { target *-*-* } }
}

fn main() {}

