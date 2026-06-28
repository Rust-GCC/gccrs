// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core)]
#![no_core]

pub struct S0(usize);

impl S0 {
    pub fn foo() -> S0 {
        fn bar() -> S0 {
            Self(0)
// { dg-warning "cannot reference .Self. constructor from outer item" "" { target *-*-* } .-1 }
        }
        bar()
    }

    pub fn direct() -> S0 {
        Self(0)
    }
}
