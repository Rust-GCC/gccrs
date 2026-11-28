use std::ops::*;

#[derive(Copy, Clone)] // { dg-error ".E0204." "" { target *-*-* } }
struct R(Range<usize>);

fn main() {}

