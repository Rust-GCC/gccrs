// ignore-tidy-linelength
// aux-build:unstable_generic_param.rs

extern crate unstable_generic_param;

use unstable_generic_param::*;

impl<T> Trait3<usize> for T where T: Trait2<usize> { // { dg-error ".E0658." "" { target *-*-* } }
    fn foo() -> usize { T::foo() }
}

fn main() {}

