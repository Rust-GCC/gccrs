#![allow(warnings)]

trait Trait<T> {
    fn foo(_: T) {}
}

pub struct Foo<T = Box<Trait<DefaultFoo>>>;  // { dg-error ".E0391." "" { target *-*-* } }
type DefaultFoo = Foo;

fn main() {
}

