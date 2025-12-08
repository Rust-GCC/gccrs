#![feature(repr_simd)]
#![allow(dead_code)]

#[repr(simd)]
#[derive(Copy, Clone)]
struct LocalSimd(u8, u8);

extern {
    fn baz() -> LocalSimd; // { dg-error "" "" { target *-*-* } }
    fn qux(x: LocalSimd); // { dg-error "" "" { target *-*-* } }
}

fn main() {}

