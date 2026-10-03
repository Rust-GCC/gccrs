// aux-build:tdticc_coherence_lib.rs

// Test that we do not consider associated types to be sendable without
// some applicable trait bound (and we don't ICE).

#![feature(negative_impls)]

extern crate tdticc_coherence_lib as lib;

use lib::DefaultedTrait;

struct A;
impl DefaultedTrait for (A,) { } // { dg-error ".E0117." "" { target *-*-* } }

struct B;
impl !DefaultedTrait for (B,) { } // { dg-error ".E0117." "" { target *-*-* } }

struct C;
struct D<T>(T);
impl DefaultedTrait for Box<C> { } // { dg-error ".E0321." "" { target *-*-* } }
impl DefaultedTrait for lib::Something<C> { } // { dg-error ".E0117." "" { target *-*-* } }
impl DefaultedTrait for D<C> { } // OK

fn main() { }

