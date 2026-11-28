// compile-flags:--crate-name=test
// aux-build:coherence_lib.rs

extern crate coherence_lib as lib;
use lib::*;
use std::rc::Rc;

struct Local;

impl<T> Remote1<u32> for T {
// { dg-error ".E0210." "" { target *-*-* } .-1 }
}

fn main() {}

