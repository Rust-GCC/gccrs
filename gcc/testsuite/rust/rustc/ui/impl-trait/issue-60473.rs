// Regression test for #60473

#![feature(impl_trait_in_bindings)]
#![allow(incomplete_features)]

struct A<'a>(&'a ());

trait Trait<T> {}

impl<T> Trait<T> for () {}

fn main() {
    let x: impl Trait<A> = ();
// { dg-error ".E0106." "" { target *-*-* } .-1 }
}

