#![deny(unreachable_patterns)]

fn main() {
    let buf = &[0, 1, 2, 3];

    match buf {
        b"AAAA" => {},
        &[0x41, 0x41, 0x41, 0x41] => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }

    match buf {
        &[0x41, 0x41, 0x41, 0x41] => {}
        b"AAAA" => {}, // { dg-error "" "" { target *-*-* } }
        _ => {}
    }

    match buf {
        &[_, 0x41, 0x41, 0x41] => {},
        b"AAAA" => {}, // { dg-error "" "" { target *-*-* } }
        _ => {}
    }

    match buf {
        &[0x41, .., 0x41] => {}
        b"AAAA" => {}, // { dg-error "" "" { target *-*-* } }
        _ => {}
    }

    let buf: &[u8] = buf;

    match buf {
        b"AAAA" => {},
        &[0x41, 0x41, 0x41, 0x41] => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }

    match buf {
        &[0x41, 0x41, 0x41, 0x41] => {}
        b"AAAA" => {}, // { dg-error "" "" { target *-*-* } }
        _ => {}
    }

    match buf {
        &[_, 0x41, 0x41, 0x41] => {},
        b"AAAA" => {}, // { dg-error "" "" { target *-*-* } }
        _ => {}
    }

    match buf {
        &[0x41, .., 0x41] => {}
        b"AAAA" => {}, // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
}

