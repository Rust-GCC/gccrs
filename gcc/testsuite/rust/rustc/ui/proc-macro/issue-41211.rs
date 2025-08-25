// aux-build:test-macros.rs

// FIXME: https://github.com/rust-lang/rust/issues/41430
// This is a temporary regression test for the ICE reported in #41211

#![feature(custom_inner_attributes)]
#![feature(register_attr)]

#![register_attr(identity_attr)]

#![identity_attr]
// { dg-error ".E0659." "" { target *-*-* } .-1 }
extern crate test_macros;
use test_macros::identity_attr;

fn main() {}

