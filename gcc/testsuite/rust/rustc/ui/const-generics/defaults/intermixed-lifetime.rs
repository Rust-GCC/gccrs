// revisions: full min
// Checks that lifetimes cannot be interspersed between consts and types.
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct Foo<const N: usize, 'a, T = u32>(&'a (), T);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

struct Bar<const N: usize, T = u32, 'a>(&'a (), T);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

