// aux-build:coherence_lib.rs

extern crate coherence_lib as lib;
use lib::Remote1;

impl<T> Remote1<T> for isize { }
// { dg-error ".E0210." "" { target *-*-* } .-1 }

fn main() { }

