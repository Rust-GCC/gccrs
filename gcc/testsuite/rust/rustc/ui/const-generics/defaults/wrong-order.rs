// revisions: full min
#![cfg_attr(full, feature(const_generics))] // { dg-warning "" "" { target *-*-* } }
#![cfg_attr(min, feature(min_const_generics))]

struct A<T = u32, const N: usize> {
// { dg-error "" "" { target *-*-* } .-1 }
    arg: T,
}

fn main() {}

