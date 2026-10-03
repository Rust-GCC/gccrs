// aux-build:lint_output_format.rs

#![feature(unstable_test_feature)]
// check-pass

extern crate lint_output_format;
use lint_output_format::{foo, bar};
// { dg-warning "" "" { target *-*-* } .-1 }


fn main() {
    let _x = foo();
// { dg-warning "" "" { target *-*-* } .-1 }
    let _y = bar();
}

