#![feature(type_alias_impl_trait)]

pub trait Bar<T> {
    type Item;
}

type Foo = impl Bar<Foo, Item = Foo>;
// { dg-error "" "" { target *-*-* } .-1 }

fn crash(x: Foo) -> Foo {
    x
}

fn main() {

}

