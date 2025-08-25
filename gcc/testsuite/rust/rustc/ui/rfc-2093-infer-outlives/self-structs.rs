#![feature(rustc_attrs)]

#[rustc_outlives]
struct Foo<'a, 'b, T> { // { dg-error "" "" { target *-*-* } }
    field1: dyn Bar<'a, 'b, T>
}

trait Bar<'x, 's, U>
    where U: 'x,
    Self:'s
{}

fn main() {}

