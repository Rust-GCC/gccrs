// ignore-tidy-linelength
// ignore-compare-mode-chalk
#![feature(arbitrary_self_types)]
#![feature(type_alias_impl_trait)]

use std::ops::Deref;

trait Foo {
    type Bar: Foo;

    fn foo(self: impl Deref<Target = Self>) -> Self::Bar;
}

impl<C> Foo for C {
    type Bar = impl Foo;

    fn foo(self: impl Deref<Target = Self>) -> Self::Bar {
// { dg-error "" "" { target *-*-* } .-1 }
        self
    }
}

fn main() {}

