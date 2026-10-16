// Test that we check supertrait bounds for WFedness.

#![feature(associated_type_defaults)]

#![allow(dead_code)]

trait ExtraCopy<T:Copy> { }

trait SomeTrait<T,U>
    where T: ExtraCopy<U> // { dg-error ".E0277." "" { target *-*-* } }
{
}


fn main() { }

