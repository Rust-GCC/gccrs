// Unstable entities should be caught in import lists

// aux-build:lint-stability.rs

#![allow(warnings)]

extern crate lint_stability;

use lint_stability::{unstable, deprecated};
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {
}

