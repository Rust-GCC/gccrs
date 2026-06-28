// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core)]
#![no_core]

extern {
// { dg-warning "extern declarations without an explicit ABI are deprecated" "" { target *-*-* } .-1 }
    fn foo();
}

extern "C" {
    fn bar();
}
