#![feature(coerce_unsized)]
use std::ops::CoerceUnsized;

struct Foo<T: ?Sized> {
    a: T,
}

impl<T, U> CoerceUnsized<U> for Foo<T> {} // { dg-error ".E0376." "" { target *-*-* } }

fn main() {}

