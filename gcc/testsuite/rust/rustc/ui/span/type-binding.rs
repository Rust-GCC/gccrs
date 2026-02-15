// Regression test for issue #28158
// Test the type binding span doesn't include >>

use std::ops::Deref;

fn homura<T: Deref<Trget = i32>>(_: T) {}
// { dg-error ".E0220." "" { target *-*-* } .-1 }

fn main() {}

