#![feature(repr_simd)]
#![allow(non_camel_case_types)]

#[repr(simd)]
struct empty; // { dg-error ".E0075." "" { target *-*-* } }

#[repr(simd)]
struct i64f64(i64, f64); // { dg-error ".E0076." "" { target *-*-* } }

fn main() {}

