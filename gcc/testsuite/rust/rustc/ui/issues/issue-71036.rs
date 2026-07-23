#![feature(unsize, dispatch_from_dyn)]

use std::marker::Unsize;
use std::ops::DispatchFromDyn;

#[allow(unused)]
struct Foo<'a, T: ?Sized> {
    _inner: &'a &'a T,
}

impl<'a, T: ?Sized + Unsize<U>, U: ?Sized> DispatchFromDyn<Foo<'a, U>> for Foo<'a, T> {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-note ".E0277." "" { target *-*-* } .-2 }
// { dg-note ".E0277." "" { target *-*-* } .-3 }
// { dg-note ".E0277." "" { target *-*-* } .-4 }

fn main() {}

