// Regression test for diagnostic issue #76191
#![allow(non_snake_case)]

use std::ops::RangeInclusive;

const RANGE: RangeInclusive<i32> = 0..=255;

const RANGE2: RangeInclusive<i32> = panic!();

fn main() {
    let n: i32 = 1;
    match n {
        RANGE => {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        RANGE2 => {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        _ => {}
    }
}

