// aux-build:derive-b.rs

#![allow(warnings)]

#[macro_use]
extern crate derive_b;

#[derive(B)]
struct A {
    a: &u64
// { dg-error ".E0106." "" { target *-*-* } .-1 }
}

fn main() {
}

