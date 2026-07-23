fn main() {
    let buf = &[0, 1, 2, 3];

    match buf { // { dg-error ".E0004." "" { target *-*-* } }
        b"AAAA" => {}
    }

    let buf: &[u8] = buf;

    match buf { // { dg-error ".E0004." "" { target *-*-* } }
        b"AAAA" => {}
    }
}

