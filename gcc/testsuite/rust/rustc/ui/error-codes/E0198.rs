#![feature(negative_impls)]

struct Foo;

unsafe impl !Send for Foo { } // { dg-error ".E0198." "" { target *-*-* } }

fn main() {
}

