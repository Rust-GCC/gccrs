#![feature(rustc_attrs)]

trait Trait<'x, T> where T: 'x {
    type Type;
}

#[rustc_outlives]
struct Foo<'a, A, B> where A: Trait<'a, B> // { dg-error "" "" { target *-*-* } }
{
    foo: <A as Trait<'a, B>>::Type
}

fn main() {}

