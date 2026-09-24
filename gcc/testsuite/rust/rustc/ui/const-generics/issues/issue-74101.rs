// [full] check-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

fn test<const N: [u8; 1 + 2]>() {}
// { dg-error "" "" { target *-*-* } .-1 }

struct Foo<const N: [u8; 1 + 2]>;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

