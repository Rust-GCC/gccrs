// aux-build:coherence_lib.rs

extern crate coherence_lib as lib;
use lib::Remote;

impl<T> Remote for T { }
// { dg-error ".E0210." "" { target *-*-* } .-1 }


fn main() { }

