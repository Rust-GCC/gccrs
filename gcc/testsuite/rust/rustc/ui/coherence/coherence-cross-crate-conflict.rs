// The error here is strictly due to orphan rules; the impl here
// generalizes the one upstream

// aux-build:trait_impl_conflict.rs

extern crate trait_impl_conflict;
use trait_impl_conflict::Foo;

impl<A> Foo for A {
// { dg-error ".E0210." "" { target *-*-* } .-1 }
// { dg-error ".E0210." "" { target *-*-* } .-2 }
}

fn main() {
}

