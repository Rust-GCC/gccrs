// run-rustfix

#![allow(dead_code)]

struct S {
    x: u8
    /// The ID of the parent core
    y: u8,
}
// { dg-error ".E0585." "" { target *-*-* } .-3 }

fn main() {}

