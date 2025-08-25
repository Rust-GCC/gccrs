// compile-flags: -Zsave-analysis
// Regression test for #69414 ^

use std::marker::PhantomData;

struct B<T, const N: T>(PhantomData<[T; N]>); // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {}

