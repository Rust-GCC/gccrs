// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core)]
#![no_core]

pub fn foo() {
    match 1 {
        _ => {}
        1 => {}
// { dg-warning "unreachable pattern" "" { target *-*-* } .-1 }
    }
}

// A bare identifier pattern is not treated as irrefutable, so the following
// arm must not be reported as unreachable.
pub fn ident_binding(x: i32) {
    match x {
        _a => {}
        1 => {}
    }
}

enum E {
    A,
    B,
}
use E::{A, B};

// A bare unit-variant pattern (parsed as an identifier) is refutable, so the
// second arm is reachable and must not be flagged.
pub fn variant(e: E) {
    match e {
        A => {}
        B => {}
    }
}
