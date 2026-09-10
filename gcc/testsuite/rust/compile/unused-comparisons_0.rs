// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core)]
#![no_core]

pub fn foo() {
    let x: u32 = 1;
    let _b = x < 0;
// { dg-warning "comparison is useless due to type limits" "" { target *-*-* } .-1 }
}
