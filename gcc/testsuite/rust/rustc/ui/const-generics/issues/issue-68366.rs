// Checks that const expressions have a useful note explaining why they can't be evaluated.
// The note should relate to the fact that it cannot be shown forall N that it maps 1-1 to a new
// type.

// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct Collatz<const N: Option<usize>>;

impl <const N: usize> Collatz<{Some(N)}> {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

struct Foo;

impl<const N: usize> Foo {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

