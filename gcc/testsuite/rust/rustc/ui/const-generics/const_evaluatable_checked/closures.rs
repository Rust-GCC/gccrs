#![feature(const_generics, const_evaluatable_checked)]
#![allow(incomplete_features)]
fn test<const N: usize>() -> [u8; N + (|| 42)()] {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

