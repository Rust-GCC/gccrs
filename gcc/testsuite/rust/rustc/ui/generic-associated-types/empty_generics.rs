#![allow(incomplete_features)]
#![feature(generic_associated_types)]

trait Foo {
    type Bar<,>;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

