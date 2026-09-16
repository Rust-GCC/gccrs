// aux-build:derive-unstable-2.rs

#![feature(register_attr)]

#![register_attr(rustc_foo)]

#[macro_use]
extern crate derive_unstable_2;

#[derive(Unstable)]
// { dg-error "" "" { target *-*-* } .-1 }

struct A;

fn main() {
    foo();
}

