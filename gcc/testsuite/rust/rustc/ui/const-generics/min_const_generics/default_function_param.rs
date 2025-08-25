#![feature(min_const_generics)]

fn foo<const SIZE: usize = 5>() {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

