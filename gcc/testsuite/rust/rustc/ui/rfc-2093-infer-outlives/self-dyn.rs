#![feature(dyn_trait)]
#![feature(rustc_attrs)]

trait Trait<'x, 's, T> where T: 'x,
      's: {
}

#[rustc_outlives]
struct Foo<'a, 'b, A> // { dg-error "" "" { target *-*-* } }
{
    foo: Box<dyn Trait<'a, 'b, A>>
}

fn main() {}

