#![feature(dyn_trait)]
#![feature(rustc_attrs)]

trait Trait<'x, T> where T: 'x {
}

#[rustc_outlives]
struct Foo<'a, A> // { dg-error "" "" { target *-*-* } }
{
    foo: Box<dyn Trait<'a, A>>
}

fn main() {}

