// Test that we check associated type default values for WFedness.

#![feature(associated_type_defaults)]

#![allow(dead_code)]

trait SomeTrait<'a> {
    type Type1;
    type Type2 = &'a Self::Type1;
// { dg-error ".E0309." "" { target *-*-* } .-1 }
}


fn main() { }

