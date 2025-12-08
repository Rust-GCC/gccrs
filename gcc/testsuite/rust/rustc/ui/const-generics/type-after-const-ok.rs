// [full] run-pass
// revisions: full min
// Verifies that having generic parameters after constants is permitted
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

#[allow(dead_code)]
struct A<const N: usize, T>(T);
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

