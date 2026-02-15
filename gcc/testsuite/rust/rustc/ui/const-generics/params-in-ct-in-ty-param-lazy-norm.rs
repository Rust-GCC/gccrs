// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct Foo<T, U = [u8; std::mem::size_of::<T>()]>(T, U);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

// FIXME(const_generics:defaults): We still don't know how to we deal with type defaults.
struct Bar<T = [u8; N], const N: usize>(T);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

