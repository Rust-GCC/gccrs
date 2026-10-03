// ignore-tidy-linelength
// ignore-compare-mode-chalk
#![feature(type_alias_impl_trait)]

pub trait Bar {
    type E: Copy;

    fn foo<T>() -> Self::E;
}

impl<S> Bar for S {
    type E = impl Copy;

    fn foo<T>() -> Self::E {
// { dg-error "" "" { target *-*-* } .-1 }
        || ()
    }
}

fn main() {}

