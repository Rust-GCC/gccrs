#![feature(no_core)]
#![no_core]

// Test for segfault when processing enum variant with reference to type
// with unresolved lifetime in generic arguments.
// The compiler should report an error gracefully instead of crashing.

enum ast<'a> {
    num(usize),
    add(&'a ast<'result>) // { dg-error "failed to resolve type path segment" }
    // { dg-error "unresolved lifetime" "" { target *-*-* } .-1 }
}

pub fn main() {}
