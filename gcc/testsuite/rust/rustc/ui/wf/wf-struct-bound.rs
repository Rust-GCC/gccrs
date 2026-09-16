// Test that we check struct bounds for WFedness.

#![feature(associated_type_defaults)]

#![allow(dead_code)]

trait ExtraCopy<T:Copy> { }

struct SomeStruct<T,U>
    where T: ExtraCopy<U> // { dg-error ".E0277." "" { target *-*-* } }
{
    data: (T,U)
}


fn main() { }

