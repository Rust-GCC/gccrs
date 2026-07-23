// run-rustfix

#[allow(unused)]
use std::fmt::Debug;
// Rustfix should add this, or use `std::fmt::Debug` instead.

#[allow(dead_code)]
struct ConstrainedStruct<X: Copy> {
    x: X
}

#[allow(dead_code)]
trait InsufficientlyConstrainedGeneric<X=()> {
    fn return_the_constrained_type(&self, x: X) -> ConstrainedStruct<X> {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        ConstrainedStruct { x }
    }
}

pub fn main() { }

