// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core)]
#![no_core]

pub fn foo() -> i32 {
    let _x = 0i32 as i32;
// { dg-warning "trivial numeric cast" "" { target *-*-* } .-1 }
    _x
}
