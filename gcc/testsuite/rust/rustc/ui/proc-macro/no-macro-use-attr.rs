// aux-build:test-macros.rs

#![feature(rustc_attrs)]
#![warn(unused_extern_crates)]

extern crate test_macros;
// { dg-warning "" "" { target *-*-* } .-1 }

#[rustc_error]
fn main() {} // { dg-error "" "" { target *-*-* } }

