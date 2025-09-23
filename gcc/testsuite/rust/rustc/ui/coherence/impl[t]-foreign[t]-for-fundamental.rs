// compile-flags:--crate-name=test
// aux-build:coherence_lib.rs

extern crate coherence_lib as lib;
use lib::*;
use std::rc::Rc;

struct Local;

impl<T> Remote1<T> for Box<T> {
// { dg-error ".E0210." "" { target *-*-* } .-1 }
}

impl<'a, A, B> Remote1<A> for &'a B {
// { dg-error ".E0210." "" { target *-*-* } .-1 }
}

fn main() {}

