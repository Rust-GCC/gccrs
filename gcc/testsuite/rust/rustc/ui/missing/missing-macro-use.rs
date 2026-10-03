// aux-build:two_macros.rs

extern crate two_macros;

pub fn main() {
    macro_two!();
// { dg-error "" "" { target *-*-* } .-1 }
}

