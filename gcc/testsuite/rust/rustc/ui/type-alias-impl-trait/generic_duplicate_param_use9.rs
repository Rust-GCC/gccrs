#![feature(type_alias_impl_trait)]

use std::fmt::Debug;

fn main() {}

type Two<A, B> = impl Debug;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }

trait Foo {
    type Bar: Debug;
    const BAR: Self::Bar;
}

fn two<T: Debug + Foo, U: Debug>(t: T, u: U) -> Two<T, U> {
    (t, u, T::BAR)
}

fn three<T: Debug, U: Debug>(t: T, u: U) -> Two<T, U> {
// { dg-error "" "" { target *-*-* } .-1 }
}

