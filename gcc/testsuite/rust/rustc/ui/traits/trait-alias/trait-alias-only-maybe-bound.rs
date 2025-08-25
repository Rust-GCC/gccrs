// Test that `dyn ?Sized` (i.e., a trait object with only a maybe buond) is not allowed, when just
// `?Sized` results from trait alias expansion.

#![feature(trait_alias)]

trait S = ?Sized;

// Nest a couple of levels deep:
trait _0 = S;
trait _1 = _0;

// Straight list expansion:
type _T0 = dyn _1;
// { dg-error ".E0224." "" { target *-*-* } .-1 }

// Twice:
trait _2 = _1 + _1;

type _T1 = dyn _2;
// { dg-error ".E0224." "" { target *-*-* } .-1 }

fn main() {}

