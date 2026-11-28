#![feature(rustc_attrs)]

#[rustc_outlives]
enum Foo<'a, T> { // { dg-error "" "" { target *-*-* } }

    One(Bar<'a, T>)
}

struct Bar<'b, U> {
    field2: &'b U
}

fn main() {}

