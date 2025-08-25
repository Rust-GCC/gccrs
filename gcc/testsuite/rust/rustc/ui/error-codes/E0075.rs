#![feature(repr_simd)]

#[repr(simd)]
struct Bad; // { dg-error ".E0075." "" { target *-*-* } }

fn main() {
}

