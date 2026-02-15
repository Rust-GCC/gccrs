// compile-flags:--crate-name=test
// aux-build:coherence_lib.rs

extern crate coherence_lib as lib;
use lib::*;
use std::rc::Rc;

struct Local;

impl<'a, T> Remote1<Box<T>> for &'a T {
// { dg-error ".E0210." "" { target *-*-* } .-1 }
}
impl<'a, T> Remote1<&'a T> for Box<T> {
// { dg-error ".E0210." "" { target *-*-* } .-1 }
}

fn main() {}

