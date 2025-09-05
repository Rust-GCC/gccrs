// Ensure that we don't ICE if associated type impl trait is used in an impl
// with an unconstrained type parameter.

#![feature(type_alias_impl_trait)]

trait X {
    type I;
    fn f() -> Self::I;
}

impl<T> X for () {
// { dg-error ".E0207." "" { target *-*-* } .-1 }
    type I = impl Sized;
    fn f() -> Self::I {}
}

fn main() {}

