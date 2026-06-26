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
