#![allow(incomplete_features)]
#![feature(const_trait_impl)]

struct S;
trait T {
    fn foo();
}

fn non_const() {}

impl const T for S {
    fn foo() { non_const() }
// { dg-error ".E0015." "" { target *-*-* } .-1 }
}

fn main() {}

