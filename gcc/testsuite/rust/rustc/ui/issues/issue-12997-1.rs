// compile-flags: --test

//! Test that makes sure wrongly-typed bench functions aren't ignored

#![feature(test)]

#[bench]
fn foo() { } // { dg-error "" "" { target *-*-* } }

#[bench]
fn bar(x: isize, y: isize) { } // { dg-error "" "" { target *-*-* } }

