// aux-build:coherence_lib.rs

extern crate coherence_lib as lib;
use lib::Remote;

struct Foo;

impl<T> Remote for lib::Pair<T,Foo> { }
// { dg-error ".E0117." "" { target *-*-* } .-1 }

fn main() { }

