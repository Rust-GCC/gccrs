// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core)]
#![no_core]

pub fn foo() {
    while true {}
// { dg-warning "denote infinite loops with" "" { target *-*-* } .-1 }
    while true == true {}
// { dg-warning "denote infinite loops with" "" { target *-*-* } .-1 }
    while false != true {}
// { dg-warning "denote infinite loops with" "" { target *-*-* } .-1 }
    while true == false {}
}
