// aux-build:derive-unstable.rs

#![allow(warnings)]

#[macro_use]
extern crate derive_unstable;

#[derive(Unstable)]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
struct A;

fn main() {
    unsafe { foo(); }
}

