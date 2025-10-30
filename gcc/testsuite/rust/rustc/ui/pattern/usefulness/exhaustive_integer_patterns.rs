#![feature(precise_pointer_size_matching)]
#![feature(exclusive_range_pattern)]
#![deny(unreachable_patterns)]
#![deny(overlapping_patterns)]

use std::{char, u8, u16, u32, u64, u128, i8, i16, i32, i64, i128};

fn main() {
    let x: u8 = 0;

    // A single range covering the entire domain.
    match x {
        0 ..= 255 => {} // ok
    }

    // A combination of ranges and values.
    // These are currently allowed to be overlapping.
    match x {
        0 ..= 32 => {}
        33 => {}
        34 .. 128 => {}
        100 ..= 200 => {}
        200 => {} // { dg-error "" "" { target *-*-* } }
        201 ..= 255 => {}
    }

    // An incomplete set of values.
    match x { // { dg-error ".E0004." "" { target *-*-* } }
        0 .. 128 => {}
    }

    // A more incomplete set of values.
    match x { // { dg-error ".E0004." "" { target *-*-* } }
        0 ..= 10 => {}
        20 ..= 30 => {}
        35 => {}
        70 .. 255 => {}
    }

    let x: i8 = 0;
    match x { // { dg-error ".E0004." "" { target *-*-* } }
        -7 => {}
        -5..=120 => {}
        -2..=20 => {}
// { dg-error "" "" { target *-*-* } .-1 }
        125 => {}
    }

    // Let's test other types too!
    let c: char = '\u{0}';
    match c {
        '\u{0}' ..= char::MAX => {} // ok
    }

    // We can actually get away with just covering the
    // following two ranges, which correspond to all
    // valid Unicode Scalar Values.
    match c {
        '\u{0000}' ..= '\u{D7FF}' => {}
        '\u{E000}' ..= '\u{10_FFFF}' => {}
    }

    match 0u16 {
        0 ..= u16::MAX => {} // ok
    }

    match 0u32 {
        0 ..= u32::MAX => {} // ok
    }

    match 0u64 {
        0 ..= u64::MAX => {} // ok
    }

    match 0u128 {
        0 ..= u128::MAX => {} // ok
    }

    match 0i8 {
        -128 ..= 127 => {} // ok
    }

    match 0i8 { // { dg-error ".E0004." "" { target *-*-* } }
        -127 ..= 127 => {}
    }

    match 0i16 {
        i16::MIN ..= i16::MAX => {} // ok
    }

    match 0i16 { // { dg-error ".E0004." "" { target *-*-* } }
        i16::MIN ..= -1 => {}
        1 ..= i16::MAX => {}
    }

    match 0i32 {
        i32::MIN ..= i32::MAX => {} // ok
    }

    match 0i64 {
        i64::MIN ..= i64::MAX => {} // ok
    }

    match 0i128 {
        i128::MIN ..= i128::MAX => {} // ok
    }

    // Make sure that guards don't factor into the exhaustiveness checks.
    match 0u8 { // { dg-error ".E0004." "" { target *-*-* } }
        0 .. 128 => {}
        128 ..= 255 if true => {}
    }

    match 0u8 {
        0 .. 128 => {}
        128 ..= 255 if false => {}
        128 ..= 255 => {} // ok, because previous arm was guarded
    }

    // Now things start getting a bit more interesting. Testing products!
    match (0u8, Some(())) { // { dg-error ".E0004." "" { target *-*-* } }
        (1, _) => {}
        (_, None) => {}
    }

    match (0u8, true) { // { dg-error ".E0004." "" { target *-*-* } }
        (0 ..= 125, false) => {}
        (128 ..= 255, false) => {}
        (0 ..= 255, true) => {}
    }

    match (0u8, true) { // ok
        (0 ..= 125, false) => {}
        (128 ..= 255, false) => {}
        (0 ..= 255, true) => {}
        (125 .. 128, false) => {}
    }

    match 0u8 {
        0 .. 2 => {}
        1 ..= 2 => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }

    const LIM: u128 = u128::MAX - 1;
    match 0u128 { // { dg-error ".E0004." "" { target *-*-* } }
        0 ..= LIM => {}
    }

    match 0u128 { // { dg-error ".E0004." "" { target *-*-* } }
        0 ..= 4 => {}
    }

    match 0u128 { // { dg-error ".E0004." "" { target *-*-* } }
        4 ..= u128::MAX => {}
    }

    const FOO: i32 = 42;
    const BAR: &i32 = &42;
    match &0 {
        &42 => {}
        &FOO => {} // { dg-error "" "" { target *-*-* } }
        BAR => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }

    // Regression test, see https://github.com/rust-lang/rust/pull/66326#issuecomment-552889933
    match &0 {
        BAR => {} // ok
        _ => {}
    }
}

