// aux-build:derive-bad.rs

#[macro_use]
extern crate derive_bad;

#[derive(A)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
struct A; // { dg-error ".E0428." "" { target *-*-* } }

fn main() {}

