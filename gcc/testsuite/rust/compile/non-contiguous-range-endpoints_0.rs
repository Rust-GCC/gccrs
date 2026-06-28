// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core, exclusive_range_pattern)]
#![no_core]

pub fn f(x: u8) -> u8 {
    match x {
        20..30 => 1,
// { dg-warning "multiple ranges are one apart" "" { target *-*-* } .-1 }
        31..=40 => 2,
        _ => 0,
    }
}

pub fn g(x: u8) -> u8 {
    match x {
        0..10 => 1,
        10..=20 => 2,
        _ => 0,
    }
}
