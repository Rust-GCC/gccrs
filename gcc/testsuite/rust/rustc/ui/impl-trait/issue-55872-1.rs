// ignore-tidy-linelength
#![feature(type_alias_impl_trait)]

pub trait Bar {
    type E: Copy;

    fn foo<T>() -> Self::E;
}

impl<S: Default> Bar for S {
    type E = impl Copy;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

    fn foo<T: Default>() -> Self::E {
// { dg-error ".E0276." "" { target *-*-* } .-1 }
// { dg-error ".E0276." "" { target *-*-* } .-2 }
        (S::default(), T::default())
    }
}

fn main() {}

