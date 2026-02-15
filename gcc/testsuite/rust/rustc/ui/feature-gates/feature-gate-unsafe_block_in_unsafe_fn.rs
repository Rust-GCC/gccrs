#![deny(unsafe_op_in_unsafe_fn)]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }

fn main() {}

