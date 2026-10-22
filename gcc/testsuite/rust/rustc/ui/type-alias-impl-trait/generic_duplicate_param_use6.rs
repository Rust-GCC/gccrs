#![feature(type_alias_impl_trait)]

use std::fmt::Debug;

fn main() {}

// test that unused generic parameters are ok
type Two<T, U> = impl Debug;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn two<T: Copy + Debug, U: Debug>(t: T, u: U) -> Two<T, U> {
    (t, t)
}

fn three<T: Copy + Debug, U: Debug>(t: T, u: U) -> Two<T, U> {
// { dg-error "" "" { target *-*-* } .-1 }
    (u, t)
}

