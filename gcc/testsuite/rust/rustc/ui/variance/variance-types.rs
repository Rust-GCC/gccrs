#![allow(dead_code)]
#![feature(rustc_attrs)]

use std::cell::Cell;

// Check that a type parameter which is only used in a trait bound is
// not considered bivariant.

#[rustc_variance]
struct InvariantMut<'a,A:'a,B:'a> { // { dg-error ".E0208." "" { target *-*-* } }
    t: &'a mut (A,B)
}

#[rustc_variance]
struct InvariantCell<A> { // { dg-error ".E0208." "" { target *-*-* } }
    t: Cell<A>
}

#[rustc_variance]
struct InvariantIndirect<A> { // { dg-error ".E0208." "" { target *-*-* } }
    t: InvariantCell<A>
}

#[rustc_variance]
struct Covariant<A> { // { dg-error ".E0208." "" { target *-*-* } }
    t: A, u: fn() -> A
}

#[rustc_variance]
struct Contravariant<A> { // { dg-error ".E0208." "" { target *-*-* } }
    t: fn(A)
}

#[rustc_variance]
enum Enum<A,B,C> { // { dg-error ".E0208." "" { target *-*-* } }
    Foo(Covariant<A>),
    Bar(Contravariant<B>),
    Zed(Covariant<C>,Contravariant<C>)
}

pub fn main() { }

