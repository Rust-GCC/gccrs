#![feature(rustc_attrs)]

#[rustc_outlives]
struct Foo<'a, T> { // { dg-error "" "" { target *-*-* } }
    bar: std::slice::IterMut<'a, T>
}

fn main() {}

