#![deny(unreachable_patterns)]

fn main() {
    let s = &[0x00; 4][..]; //Slice of any value
    const MAGIC_TEST: &[u32] = &[4, 5, 6, 7]; //Const slice to pattern match with
    match s {
        MAGIC_TEST => (),
        [0x00, 0x00, 0x00, 0x00] => (),
        [4, 5, 6, 7] => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }
    match s {
        [0x00, 0x00, 0x00, 0x00] => (),
        MAGIC_TEST => (),
        [4, 5, 6, 7] => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }
    match s {
        [0x00, 0x00, 0x00, 0x00] => (),
        [4, 5, 6, 7] => (),
        MAGIC_TEST => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }
    const FOO: [u32; 1] = [4];
    match [99] {
        [0x00] => (),
        [4] => (),
        FOO => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }
}

