// aux-build:two_macros.rs

#[macro_use(macro_one)]
extern crate two_macros;

pub fn main() {
    macro_two!();
// { dg-error "" "" { target *-*-* } .-1 }
}

