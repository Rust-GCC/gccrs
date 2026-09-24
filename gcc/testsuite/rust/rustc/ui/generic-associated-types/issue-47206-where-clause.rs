// Check that this program doesn't cause the compiler to error without output.

#![allow(incomplete_features)]
#![feature(generic_associated_types)]

trait Foo {
    type Assoc3<T>;
}

struct Bar;

impl Foo for Bar {
    type Assoc3<T> where T: Iterator = Vec<T>;
// { dg-error ".E0276." "" { target *-*-* } .-1 }
}

fn main() {}

