#![feature(coerce_unsized)]
use std::ops::CoerceUnsized;

struct Foo<T: ?Sized, U: ?Sized> {
    a: i32,
    b: T,
    c: U,
}

impl<T, U> CoerceUnsized<Foo<U, T>> for Foo<T, U> {}
// { dg-error ".E0375." "" { target *-*-* } .-1 }

fn main() {}

