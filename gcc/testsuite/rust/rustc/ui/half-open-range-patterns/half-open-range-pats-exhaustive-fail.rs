// Test various non-exhaustive matches for `X..`, `..=X` and `..X` ranges.

#![feature(half_open_range_patterns)]
#![feature(exclusive_range_pattern)]
#![allow(illegal_floating_point_literal_pattern)]

fn main() {}

macro_rules! m {
    ($s:expr, $($t:tt)+) => {
        match $s { $($t)+ => {} }
    }
}

fn floats() {
    m!(0f32, core::f32::NEG_INFINITY..); // { dg-error ".E0004." "" { target *-*-* } }
    m!(0f32, ..core::f32::INFINITY); // { dg-error ".E0004." "" { target *-*-* } }
}

fn khar() {
    const ALMOST_MAX: char = '\u{10fffe}';
    const ALMOST_MIN: char = '\u{1}';
    const VAL: char = 'a';
    const VAL_1: char = 'b';
    const VAL_2: char = 'c';
    m!('a', ..core::char::MAX); // { dg-error ".E0004." "" { target *-*-* } }
    m!('a', ..ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
    m!('a', ALMOST_MIN..); // { dg-error ".E0004." "" { target *-*-* } }
    m!('a', ..=ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
    m!('a', ..=VAL | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
    m!('a', ..VAL_1 | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
}

mod unsigned {
    fn u8() {
        const ALMOST_MAX: u8 = core::u8::MAX - 1;
        const ALMOST_MIN: u8 = core::u8::MIN + 1;
        const VAL: u8 = 42;
        const VAL_1: u8 = VAL + 1;
        const VAL_2: u8 = VAL + 2;
        m!(0, ..core::u8::MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ALMOST_MIN..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=VAL | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..VAL_1 | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
    }
    fn u16() {
        const ALMOST_MAX: u16 = core::u16::MAX - 1;
        const ALMOST_MIN: u16 = core::u16::MIN + 1;
        const VAL: u16 = 42;
        const VAL_1: u16 = VAL + 1;
        const VAL_2: u16 = VAL + 2;
        m!(0, ..core::u16::MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ALMOST_MIN..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=VAL | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..VAL_1 | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
    }
    fn u32() {
        const ALMOST_MAX: u32 = core::u32::MAX - 1;
        const ALMOST_MIN: u32 = core::u32::MIN + 1;
        const VAL: u32 = 42;
        const VAL_1: u32 = VAL + 1;
        const VAL_2: u32 = VAL + 2;
        m!(0, ..core::u32::MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ALMOST_MIN..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=VAL | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..VAL_1 | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
    }
    fn u64() {
        const ALMOST_MAX: u64 = core::u64::MAX - 1;
        const ALMOST_MIN: u64 = core::u64::MIN + 1;
        const VAL: u64 = 42;
        const VAL_1: u64 = VAL + 1;
        const VAL_2: u64 = VAL + 2;
        m!(0, ..core::u64::MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ALMOST_MIN..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=VAL | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..VAL_1 | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
    }
    fn u128() {
        const ALMOST_MAX: u128 = core::u128::MAX - 1;
        const ALMOST_MIN: u128 = core::u128::MIN + 1;
        const VAL: u128 = 42;
        const VAL_1: u128 = VAL + 1;
        const VAL_2: u128 = VAL + 2;
        m!(0, ..core::u128::MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ALMOST_MIN..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=VAL | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..VAL_1 | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
    }
}

mod signed {
    fn i8() {
        const ALMOST_MAX: i8 = core::i8::MAX - 1;
        const ALMOST_MIN: i8 = core::i8::MIN + 1;
        const VAL: i8 = 42;
        const VAL_1: i8 = VAL + 1;
        const VAL_2: i8 = VAL + 2;
        m!(0, ..core::i8::MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ALMOST_MIN..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=VAL | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..VAL_1 | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
    }
    fn i16() {
        const ALMOST_MAX: i16 = core::i16::MAX - 1;
        const ALMOST_MIN: i16 = core::i16::MIN + 1;
        const VAL: i16 = 42;
        const VAL_1: i16 = VAL + 1;
        const VAL_2: i16 = VAL + 2;
        m!(0, ..core::i16::MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ALMOST_MIN..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=VAL | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..VAL_1 | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
    }
    fn i32() {
        const ALMOST_MAX: i32 = core::i32::MAX - 1;
        const ALMOST_MIN: i32 = core::i32::MIN + 1;
        const VAL: i32 = 42;
        const VAL_1: i32 = VAL + 1;
        const VAL_2: i32 = VAL + 2;
        m!(0, ..core::i32::MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ALMOST_MIN..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=VAL | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..VAL_1 | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
    }
    fn i64() {
        const ALMOST_MAX: i64 = core::i64::MAX - 1;
        const ALMOST_MIN: i64 = core::i64::MIN + 1;
        const VAL: i64 = 42;
        const VAL_1: i64 = VAL + 1;
        const VAL_2: i64 = VAL + 2;
        m!(0, ..core::i64::MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ALMOST_MIN..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=VAL | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..VAL_1 | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
    }
    fn i128() {
        const ALMOST_MAX: i128 = core::i128::MAX - 1;
        const ALMOST_MIN: i128 = core::i128::MIN + 1;
        const VAL: i128 = 42;
        const VAL_1: i128 = VAL + 1;
        const VAL_2: i128 = VAL + 2;
        m!(0, ..core::i128::MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ALMOST_MIN..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=ALMOST_MAX); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..=VAL | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
        m!(0, ..VAL_1 | VAL_2..); // { dg-error ".E0004." "" { target *-*-* } }
    }
}

