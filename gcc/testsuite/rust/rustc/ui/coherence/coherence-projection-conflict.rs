use std::marker::PhantomData;

pub trait Foo<P> { fn foo() {} }

pub trait Bar {
    type Output: 'static;
}

impl Foo<i32> for i32 { }

impl<A:Bar> Foo<A::Output> for A { }
// { dg-error ".E0119." "" { target *-*-* } .-1 }

impl Bar for i32 {
    type Output = i32;
}

fn main() {}

