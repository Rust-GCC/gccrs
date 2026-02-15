#![feature(or_patterns)]

fn main() {
    let x = 3;

    match x {
        1 | 2 || 3 => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }

    match x {
        (1 | 2 || 3) => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }

    match (x,) {
        (1 | 2 || 3,) => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }

    struct TS(u8);

    match TS(x) {
        TS(1 | 2 || 3) => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }

    struct NS { f: u8 }

    match (NS { f: x }) {
        NS { f: 1 | 2 || 3 } => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }

    match [x] {
        [1 | 2 || 3] => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }

    match x {
        || 1 | 2 | 3 => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }
}

