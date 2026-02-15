// ignore-tidy-linelength
// ignore-compare-mode-chalk
#![feature(type_alias_impl_trait)]

use std::fmt::Debug;

pub trait Foo {
    type Item: Debug;

    fn foo<T: Debug>(_: T) -> Self::Item;
}

#[derive(Debug)]
pub struct S<T>(std::marker::PhantomData<T>);

pub struct S2;

impl Foo for S2 {
    type Item = impl Debug;

    fn foo<T: Debug>(_: T) -> Self::Item {
// { dg-error "" "" { target *-*-* } .-1 }
        S::<T>(Default::default())
    }
}

fn main() {
    S2::foo(123);
}

