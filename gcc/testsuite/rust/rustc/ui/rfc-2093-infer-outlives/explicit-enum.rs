#![feature(rustc_attrs)]

#[rustc_outlives]
enum Foo<'a, U> { // { dg-error "" "" { target *-*-* } }
    One(Bar<'a, U>)
}

struct Bar<'x, T> where T: 'x {
    x: &'x (),
    y: T,
}

fn main() {}

