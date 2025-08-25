// aux-build:coherence_lib.rs

extern crate coherence_lib as lib;
use lib::{Remote, Pair};

struct Local<T>(T);

impl<T,U> Remote for Pair<T,Local<U>> { }
// { dg-error ".E0117." "" { target *-*-* } .-1 }

fn main() { }

