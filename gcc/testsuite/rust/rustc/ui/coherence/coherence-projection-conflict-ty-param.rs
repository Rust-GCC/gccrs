// Coherence error results because we do not know whether `T: Foo<P>` or not
// for the second impl.

use std::marker::PhantomData;

pub trait Foo<P> { fn foo() {} }

impl <P, T: Foo<P>> Foo<P> for Option<T> {}

impl<T, U> Foo<T> for Option<U> { }
// { dg-error ".E0119." "" { target *-*-* } .-1 }

fn main() {}

