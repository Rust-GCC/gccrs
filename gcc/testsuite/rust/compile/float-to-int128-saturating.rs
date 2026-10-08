// { dg-additional-options "-fdump-tree-original" }
#![feature(no_core)]
#![no_core]

// Float to 128-bit int casts saturate too. The upper bound is the power of
// two 2^N, exact in f64; for f32 to u128 it is out of range and becomes +inf,
// so only +inf saturates there.

pub fn f32_to_i128(x: f32) -> i128 {
    x as i128
}

pub fn f32_to_u128(x: f32) -> u128 {
    x as u128
}

pub fn f64_to_i128(x: f64) -> i128 {
    x as i128
}

pub fn f64_to_u128(x: f64) -> u128 {
    x as u128
}

// { dg-final { scan-tree-dump-times ">= 1.70141183460469231731687303715884105728e\\+38" 2 "original" } }
// { dg-final { scan-tree-dump-times "< -1.70141183460469231731687303715884105728e\\+38" 2 "original" } }
// { dg-final { scan-tree-dump ">=  Inf" "original" } }
// { dg-final { scan-tree-dump ">= 3.40282366920938463463374607431768211456e\\+38" "original" } }
