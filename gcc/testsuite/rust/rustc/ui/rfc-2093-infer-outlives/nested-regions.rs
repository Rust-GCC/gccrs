#![feature(rustc_attrs)]

#[rustc_outlives]
struct Foo<'a, 'b, T> { // { dg-error "" "" { target *-*-* } }
    x: &'a &'b T
}

fn main() {}

