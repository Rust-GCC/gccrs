// Test that we check struct fields for WFedness.

#![feature(associated_type_defaults)]

#![allow(dead_code)]

struct IsCopy<T:Copy> {
    value: T
}

enum AnotherEnum<A> {
    AnotherVariant {
        f: IsCopy<A> // { dg-error ".E0277." "" { target *-*-* } }
    }
}


fn main() { }

