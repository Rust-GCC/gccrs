#![feature(or_patterns)]
#![deny(unreachable_patterns)]

// We wrap patterns in a tuple because top-level or-patterns were special-cased.
fn main() {
    match (0u8,) {
        (1 | 2,) => {}
        (1,) => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
    match (0u8,) {
        (1 | 2,) => {}
        (2,) => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
    match (0u8,) {
        (1,) => {}
        (2,) => {}
        (1 | 2,) => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
    match (0u8, 0u8) {
        (1 | 2, 3 | 4) => {}
        (1, 3) => {}     // { dg-error "" "" { target *-*-* } }
        (1, 4) => {}     // { dg-error "" "" { target *-*-* } }
        (2, 4) => {}     // { dg-error "" "" { target *-*-* } }
        (2 | 1, 4) => {} // { dg-error "" "" { target *-*-* } }
        (1, 5 | 6) => {}
        (1, 4 | 5) => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
    match (true, true) {
        (false | true, false | true) => (),
    }
    match (Some(0u8),) {
        (None | Some(1 | 2),) => {}
        (Some(1),) => {} // { dg-error "" "" { target *-*-* } }
        (None,) => {}    // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
    match ((0u8,),) {
        ((1 | 2,) | (3 | 4,),) => {}
        ((1..=4,),) => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }

    match (0,) {
        (1 | 1,) => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
    match [0; 2] {
        [0
            | 0 // { dg-error "" "" { target *-*-* } }
        , 0
            | 0] => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
    match &[][..] {
        [0] => {}
        [0, _] => {}
        [0, _, _] => {}
        [1, ..] => {}
        [1 // { dg-error "" "" { target *-*-* } }
            | 2, ..] => {}
        _ => {}
    }
    match Some(0) {
        Some(0) => {}
        Some(0 // { dg-error "" "" { target *-*-* } }
             | 1) => {}
        _ => {}
    }

    // A subpattern that is only unreachable in one branch is overall reachable.
    match (true, true) {
        (true, true) => {}
        (false | true, false | true) => {}
    }
    match (true, true) {
        (true, true) => {}
        (false, false) => {}
        (false | true, false | true) => {}
    }
    // https://github.com/rust-lang/rust/issues/76836
    match None {
        Some(false) => {}
        None | Some(true
                | false) => {} // { dg-error "" "" { target *-*-* } }
    }

    // A subpattern that is unreachable in all branches is overall unreachable.
    match (true, true) {
        (false, true) => {}
        (true, true) => {}
        (false | true, false
            | true) => {} // { dg-error "" "" { target *-*-* } }
    }
    match (true, true) {
        (true, false) => {}
        (true, true) => {}
        (false
            | true, // { dg-error "" "" { target *-*-* } }
            false | true) => {}
    }
}

