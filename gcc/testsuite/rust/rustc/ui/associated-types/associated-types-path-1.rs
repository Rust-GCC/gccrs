// Test that we have one and only one associated type per ref.

pub trait Foo {
    type A;
}
pub trait Bar {
    type A;
}

pub fn f1<T>(a: T, x: T::A) {} // { dg-error ".E0220." "" { target *-*-* } }
pub fn f2<T: Foo + Bar>(a: T, x: T::A) {} // { dg-error ".E0221." "" { target *-*-* } }

pub fn main() {}

