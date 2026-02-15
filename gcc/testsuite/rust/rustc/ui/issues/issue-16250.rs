#![deny(warnings)]

pub struct Foo;

extern {
    pub fn foo(x: (Foo)); // { dg-error "" "" { target *-*-* } }
}

fn main() {
}

