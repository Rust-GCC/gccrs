// check-pass

#![feature(impl_trait_in_bindings)]
// { dg-warning "" "" { target *-*-* } .-1 }

const _: impl Fn() = ||();

fn main() {}

