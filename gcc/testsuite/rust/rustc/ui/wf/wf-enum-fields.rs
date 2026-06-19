// Test that we check struct fields for WFedness.

#![feature(associated_type_defaults)]

#![allow(dead_code)]

struct IsCopy<T:Copy> {
    value: T
}

enum SomeEnum<A> {
    SomeVariant(IsCopy<A>) // { dg-error ".E0277." "" { target *-*-* } }
}


fn main() { }

