// Test the suggestion to wrap an or-pattern as a function parameter in parens.

// run-rustfix

#![feature(or_patterns)]
#![allow(warnings)]

fn main() {}

enum E { A, B }
use E::*;

#[cfg(FALSE)]
fn fun1(A | B: E) {} // { dg-error "" "" { target *-*-* } }

