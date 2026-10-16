#![feature(repr_simd)]

#[repr(simd)]
struct Bad(u16, u32, u32);
// { dg-error ".E0076." "" { target *-*-* } .-1 }

fn main() {
}

