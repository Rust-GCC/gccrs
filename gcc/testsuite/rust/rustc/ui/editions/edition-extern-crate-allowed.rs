// aux-build:edition-extern-crate-allowed.rs
// edition:2015
// check-pass

#![warn(rust_2018_idioms)]

extern crate edition_extern_crate_allowed;
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {}

