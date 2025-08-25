#![feature(rustc_attrs)]

#[rustc_outlives]
struct Foo<'a, T: Iterator> { // { dg-error "" "" { target *-*-* } }
    bar: &'a T::Item
}

fn main() {}

