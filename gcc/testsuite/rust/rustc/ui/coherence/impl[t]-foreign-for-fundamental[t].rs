// compile-flags:--crate-name=test
// aux-build:coherence_lib.rs

extern crate coherence_lib as lib;
use lib::*;
use std::rc::Rc;

struct Local;

impl<T> Remote for Box<T> {
// { dg-error ".E0210." "" { target *-*-* } .-1 }
    // | some local type (e.g., `MyStruct<T>`)
}

fn main() {}

