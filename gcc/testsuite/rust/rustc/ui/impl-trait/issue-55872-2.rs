// edition:2018
// ignore-tidy-linelength
// ignore-compare-mode-chalk

#![feature(type_alias_impl_trait)]

pub trait Bar {
    type E: Copy;

    fn foo<T>() -> Self::E;
}

impl<S> Bar for S {
    type E = impl std::marker::Copy;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    fn foo<T>() -> Self::E {
// { dg-error "" "" { target *-*-* } .-1 }
        async {}
    }
}

fn main() {}

