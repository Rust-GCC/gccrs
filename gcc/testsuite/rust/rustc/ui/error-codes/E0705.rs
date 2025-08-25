// check-pass

// This is a stub feature that doesn't control anything, so to make tidy happy,
// gate-test-test_2018_feature

#![feature(test_2018_feature)]
// { dg-warning "" "" { target *-*-* } .-1 }
#![feature(rust_2018_preview)]

fn main() {}

