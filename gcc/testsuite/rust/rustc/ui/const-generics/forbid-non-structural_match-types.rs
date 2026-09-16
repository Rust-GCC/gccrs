// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

#[derive(PartialEq, Eq)]
struct A;

struct B<const X: A>; // ok
// { dg-error "" "" { target *-*-* } .-1 }

struct C;

struct D<const X: C>; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

