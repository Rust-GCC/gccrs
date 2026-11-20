#![feature(universal_impl_trait)]

use std::fmt::Debug;

fn foo<T>(x: impl Debug) { }

fn main() {
    foo::<String>('a'); // { dg-error ".E0632." "" { target *-*-* } }
}

