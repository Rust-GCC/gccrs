// Tests that we consider `i16: Remote` to be ambiguous, even
// though the upstream crate doesn't implement it for now.

// aux-build:coherence_lib.rs


extern crate coherence_lib;

use coherence_lib::Remote;

trait Foo {}
impl<T> Foo for T where T: Remote {}
impl Foo for i16 {}
// { dg-error ".E0119." "" { target *-*-* } .-1 }

fn main() {}

