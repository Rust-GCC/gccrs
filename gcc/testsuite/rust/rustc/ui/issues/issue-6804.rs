// Matching against NaN should result in a warning

#![allow(unused)]
#![deny(illegal_floating_point_literal_pattern)]

use std::f64::NAN;

fn main() {
    let x = NAN;
    match x {
        NAN => {}, // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
        _ => {},
    };

    match [x, 1.0] {
        [NAN, _] => {}, // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
        _ => {},
    };
}

