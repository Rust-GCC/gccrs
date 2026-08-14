#![deny(unreachable_patterns)]

fn main() {
    while let 0..=2 | 1 = 0 {} // { dg-error "" "" { target *-*-* } }
    if let 0..=2 | 1 = 0 {} // { dg-error "" "" { target *-*-* } }

    match 0u8 {
        0
            | 0 => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
    match Some(0u8) {
        Some(0)
            | Some(0) => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
    match (0u8, 0u8) {
        (0, _) | (_, 0) => {}
        (0, 0) => {} // { dg-error "" "" { target *-*-* } }
        (1, 1) => {}
        _ => {}
    }
    match (0u8, 0u8) {
        (0, 1) | (2, 3) => {}
        (0, 3) => {}
        (2, 1) => {}
        _ => {}
    }
    match (0u8, 0u8) {
        (_, 0) | (_, 1) => {}
        _ => {}
    }
    match (0u8, 0u8) {
        (0, _) | (1, _) => {}
        _ => {}
    }
    match Some(0u8) {
        None | Some(_) => {}
        _ => {} // { dg-error "" "" { target *-*-* } }
    }
    match Some(0u8) {
        None | Some(_) => {}
        Some(_) => {} // { dg-error "" "" { target *-*-* } }
        None => {} // { dg-error "" "" { target *-*-* } }
    }
    match Some(0u8) {
        Some(_) => {}
        None => {}
        None | Some(_) => {} // { dg-error "" "" { target *-*-* } }
    }
    match 0u8 {
        1 | 2 => {},
        1..=2 => {}, // { dg-error "" "" { target *-*-* } }
        _ => {},
    }
}

