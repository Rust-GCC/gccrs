// Test that we check the types of constants are well-formed.

#![feature(associated_type_defaults)]

#![allow(dead_code)]

struct IsCopy<T:Copy> { t: T }
struct NotCopy;

const FOO: IsCopy<Option<NotCopy>> = IsCopy { t: None };
// { dg-error ".E0277." "" { target *-*-* } .-1 }


fn main() { }

