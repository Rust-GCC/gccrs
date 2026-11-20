#![allow(warnings)]

trait MyTrait { }

struct Foo<'a> { x: &'a u32 }

impl MyTrait for Foo {
// { dg-error ".E0726." "" { target *-*-* } .-1 }
}

fn main() {}

