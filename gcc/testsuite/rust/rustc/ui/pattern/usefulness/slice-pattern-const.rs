#![deny(unreachable_patterns)]

fn main() {
    let s = &[0x00; 4][..]; //Slice of any value
    const MAGIC_TEST: &[u8] = b"TEST"; //Const slice to pattern match with
    match s {
        MAGIC_TEST => (),
        [0x00, 0x00, 0x00, 0x00] => (),
        [84, 69, 83, 84] => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }
    match s {
        [0x00, 0x00, 0x00, 0x00] => (),
        MAGIC_TEST => (),
        [84, 69, 83, 84] => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }
    match s {
        [0x00, 0x00, 0x00, 0x00] => (),
        [84, 69, 83, 84] => (),
        MAGIC_TEST => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }
    const FOO: [u8; 1] = [4];
    match [99] {
        [0x00] => (),
        [4] => (),
        FOO => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }
    const BAR: &[u8; 1] = &[4];
    match &[99] {
        [0x00] => (),
        [4] => (),
        BAR => (), // { dg-error "" "" { target *-*-* } }
        b"a" => (),
        _ => (),
    }

    const BOO: &[u8; 0] = &[];
    match &[] {
        [] => (),
        BOO => (), // { dg-error "" "" { target *-*-* } }
        b"" => (), // { dg-error "" "" { target *-*-* } }
        _ => (), // { dg-error "" "" { target *-*-* } }
    }

    const CONST1: &[bool; 1] = &[true];
    match &[false] {
        CONST1 => {}
        [true] => {} // { dg-error "" "" { target *-*-* } }
        [false] => {}
    }
}

