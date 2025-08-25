#![feature(exclusive_range_pattern)]

use std::usize::MAX;

fn main() {
    match 0usize { // { dg-error ".E0004." "" { target *-*-* } }
        0..=MAX => {}
    }

    match 0isize { // { dg-error ".E0004." "" { target *-*-* } }
        1..=20 => {}
        -5..3 => {}
    }
}

