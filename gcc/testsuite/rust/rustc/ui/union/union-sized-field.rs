#![feature(untagged_unions)]

union Foo<T: ?Sized> {
    value: T,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

struct Foo2<T: ?Sized> {
    value: T,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    t: u32,
}

enum Foo3<T: ?Sized> {
    Value(T),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {}

