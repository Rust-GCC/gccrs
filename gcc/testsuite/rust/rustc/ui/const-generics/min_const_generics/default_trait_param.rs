#![feature(min_const_generics)]

trait Foo<const KIND: bool = true> {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

