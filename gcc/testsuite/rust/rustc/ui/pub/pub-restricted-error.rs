#![feature(pub_restricted)]

struct Bar(pub(()));

struct Foo {
    pub(crate) () foo: usize, // { dg-error "" "" { target *-*-* } }
}

fn main() {}

