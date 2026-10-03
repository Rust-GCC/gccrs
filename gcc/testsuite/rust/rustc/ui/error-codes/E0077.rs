#![feature(repr_simd)]

#[repr(simd)]
struct Bad(String); // { dg-error ".E0077." "" { target *-*-* } }

fn main() {
}

