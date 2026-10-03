#![feature(rustc_attrs)]

#[rustc_outlives]
struct Foo<'a, T> { // { dg-error "" "" { target *-*-* } }
    bar: &'a T,
}

fn main() {}

