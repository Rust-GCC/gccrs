// Test that we check associated type bounds for WFedness.

#![feature(associated_type_defaults)]

#![allow(dead_code)]

trait ExtraCopy<T:Copy> { }

trait SomeTrait<T> {
    type Type1: ExtraCopy<T>; // { dg-error ".E0277." "" { target *-*-* } }
}


fn main() { }

