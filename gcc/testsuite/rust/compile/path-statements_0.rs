// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core)]
#![no_core]

pub fn foo() {
    let x = 5;
    x;
// { dg-warning "path statement with no effect" "" { target *-*-* } .-1 }
}

pub fn bar(y: i32) -> i32 {
    y
}
