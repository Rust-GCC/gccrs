// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core)]
#![no_core]

pub fn foo(x: i32) {
    match x {
        1...5 => {}
// { dg-warning "range patterns are deprecated" "" { target *-*-* } .-1 }
        _ => {}
    }
}
