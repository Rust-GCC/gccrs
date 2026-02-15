#![feature(type_alias_impl_trait)]

fn main() {}

type Underconstrained<T: std::fmt::Debug> = impl 'static;
// { dg-error "" "" { target *-*-* } .-1 }

// not a defining use, because it doesn't define *all* possible generics
fn underconstrained<U>(_: U) -> Underconstrained<U> {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    5u32
}

type Underconstrained2<T: std::fmt::Debug> = impl 'static;
// { dg-error "" "" { target *-*-* } .-1 }

// not a defining use, because it doesn't define *all* possible generics
fn underconstrained2<U, V>(_: U, _: V) -> Underconstrained2<V> {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    5u32
}

