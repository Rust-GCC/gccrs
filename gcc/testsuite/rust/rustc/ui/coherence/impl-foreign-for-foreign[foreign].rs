// compile-flags:--crate-name=test
// aux-build:coherence_lib.rs

extern crate coherence_lib as lib;
use lib::*;
use std::rc::Rc;

struct Local;

impl Remote1<Rc<i32>> for i32 {
// { dg-error ".E0117." "" { target *-*-* } .-1 }
    // | can be implemented for arbitrary types [E0117]
}
impl Remote1<Rc<Local>> for f64 {
// { dg-error ".E0117." "" { target *-*-* } .-1 }
    // | can be implemented for arbitrary types [E0117]
}
impl<T> Remote1<Rc<T>> for f32 {
// { dg-error ".E0117." "" { target *-*-* } .-1 }
    // | can be implemented for arbitrary types [E0117]
}

fn main() {}

