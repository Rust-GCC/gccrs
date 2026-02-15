// compile-flags: -F unused_features
// aux-build:lint_output_format.rs

#![allow(deprecated)]

extern crate lint_output_format; // { dg-error ".E0658." "" { target *-*-* } }
use lint_output_format::{foo, bar}; // { dg-error ".E0658." "" { target *-*-* } }

fn main() {
    let _x = foo();
    let _y = bar(); // { dg-error ".E0658." "" { target *-*-* } }
}

