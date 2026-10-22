pub fn main() {
    let sl: &[u8] = b"foo";

    match sl { // { dg-error ".E0004." "" { target *-*-* } }
        [first, remainder @ ..] => {},
    };
}

