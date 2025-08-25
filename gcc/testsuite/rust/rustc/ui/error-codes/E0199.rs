#![feature(negative_impls)]

struct Foo;

trait Bar { }
unsafe impl Bar for Foo { } // { dg-error ".E0199." "" { target *-*-* } }

fn main() {
}

