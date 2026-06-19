// compile-flags: --test

//! Test that makes sure wrongly-typed bench functions are rejected

#![feature(test)]

#[bench]
fn bar(x: isize) { }
// { dg-error ".E0308." "" { target *-*-* } .-1 }

