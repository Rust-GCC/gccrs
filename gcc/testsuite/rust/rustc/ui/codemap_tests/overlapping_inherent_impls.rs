// Test that you cannot define items with the same name in overlapping inherent
// impl blocks.

#![allow(unused)]

struct Foo;

impl Foo {
    fn id() {} // { dg-error ".E0592." "" { target *-*-* } }
}

impl Foo {
    fn id() {}
}

struct Bar<T>(T);

impl<T> Bar<T> {
    fn bar(&self) {} // { dg-error ".E0592." "" { target *-*-* } }
}

impl Bar<u32> {
    fn bar(&self) {}
}

struct Baz<T>(T);

impl<T: Copy> Baz<T> {
    fn baz(&self) {} // { dg-error ".E0592." "" { target *-*-* } }
}

impl<T> Baz<Vec<T>> {
    fn baz(&self) {}
}

fn main() {}

