use std::fmt::Debug;

trait Foo {
    fn foo<A: Debug>(&self, a: &A, b: &impl Debug);
}

impl Foo for () {
    fn foo<B: Debug>(&self, a: &impl Debug, b: &B) { }
// { dg-error ".E0053." "" { target *-*-* } .-1 }
}

fn main() {}

