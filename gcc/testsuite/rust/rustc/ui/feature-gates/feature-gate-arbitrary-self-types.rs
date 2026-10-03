use std::{
    ops::Deref,
};

struct Ptr<T: ?Sized>(Box<T>);

impl<T: ?Sized> Deref for Ptr<T> {
    type Target = T;

    fn deref(&self) -> &T {
        &*self.0
    }
}

trait Foo {
    fn foo(self: Ptr<Self>); // { dg-error ".E0658." "" { target *-*-* } }
}

struct Bar;

impl Foo for Bar {
    fn foo(self: Ptr<Self>) {} // { dg-error ".E0658." "" { target *-*-* } }
}

impl Bar {
    fn bar(self: Box<Ptr<Self>>) {} // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {}

