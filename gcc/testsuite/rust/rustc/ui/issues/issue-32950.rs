#![feature(concat_idents)]

#[derive(Debug)]
struct Baz<T>(
    concat_idents!(Foo, Bar) // { dg-error ".E0412." "" { target *-*-* } }
// { dg-error ".E0412." "" { target *-*-* } .-1 }
);

fn main() {}

