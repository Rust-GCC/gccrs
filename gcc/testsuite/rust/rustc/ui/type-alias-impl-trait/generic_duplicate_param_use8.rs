#![feature(type_alias_impl_trait)]

use std::fmt::Debug;

fn main() {}

type Two<T, U> = impl Debug;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn two<T: Debug, U: Debug>(t: T, _: U) -> Two<T, U> {
    (t, 4u32)
}

fn three<T: Debug, U: Debug>(_: T, u: U) -> Two<T, U> {
// { dg-error "" "" { target *-*-* } .-1 }
    (u, 4u32)
}

