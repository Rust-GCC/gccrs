#![feature(no_core)]
#![no_core]

// Float to int casts saturate: NaN is 0 and out of range values become
// the integer type's MIN or MAX. Expected values from rustc 1.95.

#[inline(never)]
fn f32_to_i32(x: f32) -> i32 {
    x as i32
}

#[inline(never)]
fn f32_to_i64(x: f32) -> i64 {
    x as i64
}

#[inline(never)]
fn f32_to_i8(x: f32) -> i8 {
    x as i8
}

#[inline(never)]
fn f32_to_u32(x: f32) -> u32 {
    x as u32
}

#[inline(never)]
fn f32_to_u64(x: f32) -> u64 {
    x as u64
}

#[inline(never)]
fn f32_to_u8(x: f32) -> u8 {
    x as u8
}

#[inline(never)]
fn f64_to_i32(x: f64) -> i32 {
    x as i32
}

#[inline(never)]
fn f64_to_i64(x: f64) -> i64 {
    x as i64
}

#[inline(never)]
fn f64_to_i8(x: f64) -> i8 {
    x as i8
}

#[inline(never)]
fn f64_to_u32(x: f64) -> u32 {
    x as u32
}

#[inline(never)]
fn f64_to_u64(x: f64) -> u64 {
    x as u64
}

#[inline(never)]
fn f64_to_u8(x: f64) -> u8 {
    x as u8
}

fn main() -> i32 {
    // nan f32 as i8
    if f32_to_i8(0.0 / 0.0) != 0 {
        return 1;
    }
    // +inf f32 as i8
    if f32_to_i8(1.0 / 0.0) != 127 {
        return 2;
    }
    // -inf f32 as i8
    if f32_to_i8(-1.0 / 0.0) != -128 {
        return 3;
    }
    // max+1 f32 as i8
    if f32_to_i8(128.0) != 127 {
        return 4;
    }
    // min-1 f32 as i8
    if f32_to_i8(-129.0) != -128 {
        return 5;
    }
    // in range f32 as i8
    if f32_to_i8(-100.75) != -100 {
        return 6;
    }
    // nan f32 as u8
    if f32_to_u8(0.0 / 0.0) != 0 {
        return 7;
    }
    // +inf f32 as u8
    if f32_to_u8(1.0 / 0.0) != 255 {
        return 8;
    }
    // -inf f32 as u8
    if f32_to_u8(-1.0 / 0.0) != 0 {
        return 9;
    }
    // max+1 f32 as u8
    if f32_to_u8(256.0) != 255 {
        return 10;
    }
    // min-1 f32 as u8
    if f32_to_u8(-1.0) != 0 {
        return 11;
    }
    // in range f32 as u8
    if f32_to_u8(100.75) != 100 {
        return 12;
    }
    // nan f32 as i32
    if f32_to_i32(0.0 / 0.0) != 0 {
        return 13;
    }
    // +inf f32 as i32
    if f32_to_i32(1.0 / 0.0) != 2147483647 {
        return 14;
    }
    // -inf f32 as i32
    if f32_to_i32(-1.0 / 0.0) != -2147483648 {
        return 15;
    }
    // max+1 f32 as i32
    if f32_to_i32(2147483648.0) != 2147483647 {
        return 16;
    }
    // min-1 f32 as i32
    if f32_to_i32(-2147483649.0) != -2147483648 {
        return 17;
    }
    // in range f32 as i32
    if f32_to_i32(-100.75) != -100 {
        return 18;
    }
    // nan f32 as u32
    if f32_to_u32(0.0 / 0.0) != 0 {
        return 19;
    }
    // +inf f32 as u32
    if f32_to_u32(1.0 / 0.0) != 4294967295 {
        return 20;
    }
    // -inf f32 as u32
    if f32_to_u32(-1.0 / 0.0) != 0 {
        return 21;
    }
    // max+1 f32 as u32
    if f32_to_u32(4294967296.0) != 4294967295 {
        return 22;
    }
    // min-1 f32 as u32
    if f32_to_u32(-1.0) != 0 {
        return 23;
    }
    // in range f32 as u32
    if f32_to_u32(100.75) != 100 {
        return 24;
    }
    // nan f32 as i64
    if f32_to_i64(0.0 / 0.0) != 0 {
        return 25;
    }
    // +inf f32 as i64
    if f32_to_i64(1.0 / 0.0) != 9223372036854775807 {
        return 26;
    }
    // -inf f32 as i64
    if f32_to_i64(-1.0 / 0.0) != -9223372036854775808 {
        return 27;
    }
    // max+1 f32 as i64
    if f32_to_i64(9223372036854775808.0) != 9223372036854775807 {
        return 28;
    }
    // min-1 f32 as i64
    if f32_to_i64(-9223372036854775809.0) != -9223372036854775808 {
        return 29;
    }
    // in range f32 as i64
    if f32_to_i64(-100.75) != -100 {
        return 30;
    }
    // nan f32 as u64
    if f32_to_u64(0.0 / 0.0) != 0 {
        return 31;
    }
    // +inf f32 as u64
    if f32_to_u64(1.0 / 0.0) != 18446744073709551615 {
        return 32;
    }
    // -inf f32 as u64
    if f32_to_u64(-1.0 / 0.0) != 0 {
        return 33;
    }
    // max+1 f32 as u64
    if f32_to_u64(18446744073709551616.0) != 18446744073709551615 {
        return 34;
    }
    // min-1 f32 as u64
    if f32_to_u64(-1.0) != 0 {
        return 35;
    }
    // in range f32 as u64
    if f32_to_u64(100.75) != 100 {
        return 36;
    }
    // nan f64 as i8
    if f64_to_i8(0.0 / 0.0) != 0 {
        return 37;
    }
    // +inf f64 as i8
    if f64_to_i8(1.0 / 0.0) != 127 {
        return 38;
    }
    // -inf f64 as i8
    if f64_to_i8(-1.0 / 0.0) != -128 {
        return 39;
    }
    // max+1 f64 as i8
    if f64_to_i8(128.0) != 127 {
        return 40;
    }
    // min-1 f64 as i8
    if f64_to_i8(-129.0) != -128 {
        return 41;
    }
    // in range f64 as i8
    if f64_to_i8(-100.75) != -100 {
        return 42;
    }
    // nan f64 as u8
    if f64_to_u8(0.0 / 0.0) != 0 {
        return 43;
    }
    // +inf f64 as u8
    if f64_to_u8(1.0 / 0.0) != 255 {
        return 44;
    }
    // -inf f64 as u8
    if f64_to_u8(-1.0 / 0.0) != 0 {
        return 45;
    }
    // max+1 f64 as u8
    if f64_to_u8(256.0) != 255 {
        return 46;
    }
    // min-1 f64 as u8
    if f64_to_u8(-1.0) != 0 {
        return 47;
    }
    // in range f64 as u8
    if f64_to_u8(100.75) != 100 {
        return 48;
    }
    // nan f64 as i32
    if f64_to_i32(0.0 / 0.0) != 0 {
        return 49;
    }
    // +inf f64 as i32
    if f64_to_i32(1.0 / 0.0) != 2147483647 {
        return 50;
    }
    // -inf f64 as i32
    if f64_to_i32(-1.0 / 0.0) != -2147483648 {
        return 51;
    }
    // max+1 f64 as i32
    if f64_to_i32(2147483648.0) != 2147483647 {
        return 52;
    }
    // min-1 f64 as i32
    if f64_to_i32(-2147483649.0) != -2147483648 {
        return 53;
    }
    // in range f64 as i32
    if f64_to_i32(-100.75) != -100 {
        return 54;
    }
    // nan f64 as u32
    if f64_to_u32(0.0 / 0.0) != 0 {
        return 55;
    }
    // +inf f64 as u32
    if f64_to_u32(1.0 / 0.0) != 4294967295 {
        return 56;
    }
    // -inf f64 as u32
    if f64_to_u32(-1.0 / 0.0) != 0 {
        return 57;
    }
    // max+1 f64 as u32
    if f64_to_u32(4294967296.0) != 4294967295 {
        return 58;
    }
    // min-1 f64 as u32
    if f64_to_u32(-1.0) != 0 {
        return 59;
    }
    // in range f64 as u32
    if f64_to_u32(100.75) != 100 {
        return 60;
    }
    // nan f64 as i64
    if f64_to_i64(0.0 / 0.0) != 0 {
        return 61;
    }
    // +inf f64 as i64
    if f64_to_i64(1.0 / 0.0) != 9223372036854775807 {
        return 62;
    }
    // -inf f64 as i64
    if f64_to_i64(-1.0 / 0.0) != -9223372036854775808 {
        return 63;
    }
    // max+1 f64 as i64
    if f64_to_i64(9223372036854775808.0) != 9223372036854775807 {
        return 64;
    }
    // min-1 f64 as i64
    if f64_to_i64(-9223372036854775809.0) != -9223372036854775808 {
        return 65;
    }
    // in range f64 as i64
    if f64_to_i64(-100.75) != -100 {
        return 66;
    }
    // nan f64 as u64
    if f64_to_u64(0.0 / 0.0) != 0 {
        return 67;
    }
    // +inf f64 as u64
    if f64_to_u64(1.0 / 0.0) != 18446744073709551615 {
        return 68;
    }
    // -inf f64 as u64
    if f64_to_u64(-1.0 / 0.0) != 0 {
        return 69;
    }
    // max+1 f64 as u64
    if f64_to_u64(18446744073709551616.0) != 18446744073709551615 {
        return 70;
    }
    // min-1 f64 as u64
    if f64_to_u64(-1.0) != 0 {
        return 71;
    }
    // in range f64 as u64
    if f64_to_u64(100.75) != 100 {
        return 72;
    }
    // below 2^63 f32 as i64
    if f32_to_i64(9223371487098961920.0) != 9223371487098961920 {
        return 73;
    }
    // below 2^63 f64 as i64
    if f64_to_i64(9223372036854774784.0) != 9223372036854774784 {
        return 74;
    }
    // below 2^64 f32 as u64
    if f32_to_u64(18446742974197923840.0) != 18446742974197923840 {
        return 75;
    }
    // below 2^64 f64 as u64
    if f64_to_u64(18446744073709549568.0) != 18446744073709549568 {
        return 76;
    }
    // 2^63 f32 as i64
    if f32_to_i64(9223372036854775808.0) != 9223372036854775807 {
        return 77;
    }
    0
}
