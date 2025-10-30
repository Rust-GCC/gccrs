#![feature(core_intrinsics)]
#![feature(const_int_unchecked_arith)]

use std::intrinsics;

// The documentation of `unchecked_shl` states that it:
//
// Performs an unchecked left shift, resulting in undefined behavior when
// y < 0 or y >= N, where N is the width of T in bits.
//
// So we check this for a few `y`.

// unsigned types:

const SHL_U8: u8 = unsafe { intrinsics::unchecked_shl(5_u8, 8) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_U16: u16 = unsafe { intrinsics::unchecked_shl(5_u16, 16) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_U32: u32 = unsafe { intrinsics::unchecked_shl(5_u32, 32) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_U64: u64 = unsafe { intrinsics::unchecked_shl(5_u64, 64) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_U128: u128 = unsafe { intrinsics::unchecked_shl(5_u128, 128) };
// { dg-error "" "" { target *-*-* } .-1 }

// signed types:

const SHL_I8: i8 = unsafe { intrinsics::unchecked_shl(5_i8, 8) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I16: i16 = unsafe { intrinsics::unchecked_shl(5_16, 16) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I32: i32 = unsafe { intrinsics::unchecked_shl(5_i32, 32) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I64: i64 = unsafe { intrinsics::unchecked_shl(5_i64, 64) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I128: i128 = unsafe { intrinsics::unchecked_shl(5_i128, 128) };
// { dg-error "" "" { target *-*-* } .-1 }

// and make sure we capture y < 0:

const SHL_I8_NEG: i8 = unsafe { intrinsics::unchecked_shl(5_i8, -1) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I16_NEG: i16 = unsafe { intrinsics::unchecked_shl(5_16, -1) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I32_NEG: i32 = unsafe { intrinsics::unchecked_shl(5_i32, -1) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I64_NEG: i64 = unsafe { intrinsics::unchecked_shl(5_i64, -1) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I128_NEG: i128 = unsafe { intrinsics::unchecked_shl(5_i128, -1) };
// { dg-error "" "" { target *-*-* } .-1 }

// and that there's no special relation to the value -1 by picking some
// negative values at random:

const SHL_I8_NEG_RANDOM: i8 = unsafe { intrinsics::unchecked_shl(5_i8, -6) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I16_NEG_RANDOM: i16 = unsafe { intrinsics::unchecked_shl(5_16, -13) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I32_NEG_RANDOM: i32 = unsafe { intrinsics::unchecked_shl(5_i32, -25) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I64_NEG_RANDOM: i64 = unsafe { intrinsics::unchecked_shl(5_i64, -30) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHL_I128_NEG_RANDOM: i128 = unsafe { intrinsics::unchecked_shl(5_i128, -93) };
// { dg-error "" "" { target *-*-* } .-1 }

// Repeat it all over for `unchecked_shr`

// unsigned types:

const SHR_U8: u8 = unsafe { intrinsics::unchecked_shr(5_u8, 8) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_U16: u16 = unsafe { intrinsics::unchecked_shr(5_u16, 16) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_U32: u32 = unsafe { intrinsics::unchecked_shr(5_u32, 32) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_U64: u64 = unsafe { intrinsics::unchecked_shr(5_u64, 64) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_U128: u128 = unsafe { intrinsics::unchecked_shr(5_u128, 128) };
// { dg-error "" "" { target *-*-* } .-1 }

// signed types:

const SHR_I8: i8 = unsafe { intrinsics::unchecked_shr(5_i8, 8) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I16: i16 = unsafe { intrinsics::unchecked_shr(5_16, 16) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I32: i32 = unsafe { intrinsics::unchecked_shr(5_i32, 32) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I64: i64 = unsafe { intrinsics::unchecked_shr(5_i64, 64) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I128: i128 = unsafe { intrinsics::unchecked_shr(5_i128, 128) };
// { dg-error "" "" { target *-*-* } .-1 }

// and make sure we capture y < 0:

const SHR_I8_NEG: i8 = unsafe { intrinsics::unchecked_shr(5_i8, -1) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I16_NEG: i16 = unsafe { intrinsics::unchecked_shr(5_16, -1) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I32_NEG: i32 = unsafe { intrinsics::unchecked_shr(5_i32, -1) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I64_NEG: i64 = unsafe { intrinsics::unchecked_shr(5_i64, -1) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I128_NEG: i128 = unsafe { intrinsics::unchecked_shr(5_i128, -1) };
// { dg-error "" "" { target *-*-* } .-1 }

// and that there's no special relation to the value -1 by picking some
// negative values at random:

const SHR_I8_NEG_RANDOM: i8 = unsafe { intrinsics::unchecked_shr(5_i8, -6) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I16_NEG_RANDOM: i16 = unsafe { intrinsics::unchecked_shr(5_16, -13) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I32_NEG_RANDOM: i32 = unsafe { intrinsics::unchecked_shr(5_i32, -25) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I64_NEG_RANDOM: i64 = unsafe { intrinsics::unchecked_shr(5_i64, -30) };
// { dg-error "" "" { target *-*-* } .-1 }
const SHR_I128_NEG_RANDOM: i128 = unsafe { intrinsics::unchecked_shr(5_i128, -93) };
// { dg-error "" "" { target *-*-* } .-1 }

// Other arithmetic functions:

const _: u16 = unsafe { std::intrinsics::unchecked_add(40000u16, 30000) };
// { dg-error "" "" { target *-*-* } .-1 }

const _: u32 = unsafe { std::intrinsics::unchecked_sub(14u32, 22) };
// { dg-error "" "" { target *-*-* } .-1 }

const _: u16 = unsafe { std::intrinsics::unchecked_mul(300u16, 250u16) };
// { dg-error "" "" { target *-*-* } .-1 }

const _: i32 = unsafe { std::intrinsics::unchecked_div(1, 0) };
// { dg-error "" "" { target *-*-* } .-1 }
const _: i32 = unsafe { std::intrinsics::unchecked_div(i32::MIN, -1) };
// { dg-error "" "" { target *-*-* } .-1 }

const _: i32 = unsafe { std::intrinsics::unchecked_rem(1, 0) };
// { dg-error "" "" { target *-*-* } .-1 }
const _: i32 = unsafe { std::intrinsics::unchecked_rem(i32::MIN, -1) };
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

