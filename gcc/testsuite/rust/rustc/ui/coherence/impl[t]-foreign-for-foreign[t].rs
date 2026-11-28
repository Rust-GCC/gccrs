// compile-flags:--crate-name=test
// aux-build:coherence_lib.rs

extern crate coherence_lib as lib;
use lib::*;
use std::rc::Rc;
use std::sync::Arc;

struct Local;

impl Remote for Rc<Local> {
// { dg-error ".E0117." "" { target *-*-* } .-1 }
    // | can be implemented for arbitrary types [E0117]
}

impl<T> Remote for Arc<T> {
// { dg-error ".E0117." "" { target *-*-* } .-1 }
    // | can be implemented for arbitrary types [E0117]
}

fn main() {}

