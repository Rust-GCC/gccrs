// run-rustfix

#![allow(unused_assignments)]
#![allow(unused_variables)]
#![allow(dead_code)]
#![deny(unused_mut)]

struct Foo;
impl Foo {
    fn foo(mut self) {} // { dg-error "" "" { target *-*-* } }
    fn bar(mut self: Box<Foo>) {} // { dg-error "" "" { target *-*-* } }
}

fn main() {}

