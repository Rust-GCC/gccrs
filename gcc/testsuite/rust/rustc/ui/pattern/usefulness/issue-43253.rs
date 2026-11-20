// check-pass
#![feature(exclusive_range_pattern)]
#![warn(unreachable_patterns)]
#![warn(overlapping_patterns)]

fn main() {
    // These cases should generate no warning.
    match 10 {
        1..10 => {},
        10 => {},
        _ => {},
    }

    match 10 {
        1..10 => {},
        9..=10 => {}, // { dg-warning "" "" { target *-*-* } }
        _ => {},
    }

    match 10 {
        1..10 => {},
        10..=10 => {},
        _ => {},
    }

    // These cases should generate "unreachable pattern" warnings.
    match 10 {
        1..10 => {},
        9 => {}, // { dg-warning "" "" { target *-*-* } }
        _ => {},
    }

    match 10 {
        1..10 => {},
        8..=9 => {}, // { dg-warning "" "" { target *-*-* } }
        _ => {},
    }

    match 10 {
        5..7 => {},
        6 => {}, // { dg-warning "" "" { target *-*-* } }
        1..10 => {},
        9..=9 => {}, // { dg-warning "" "" { target *-*-* } }
        6 => {}, // { dg-warning "" "" { target *-*-* } }
        _ => {},
    }
}

