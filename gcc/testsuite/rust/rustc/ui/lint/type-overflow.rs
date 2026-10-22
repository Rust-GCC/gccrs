// check-pass
#![warn(overflowing_literals)]

fn main() {
    let error = 255i8; // { dg-warning "" "" { target *-*-* } }

    let ok = 0b1000_0001; // should be ok -> i32
    let ok = 0b0111_1111i8; // should be ok -> 127i8

    let fail = 0b1000_0001i8; // { dg-warning "" "" { target *-*-* } }

    let fail = 0x8000_0000_0000_0000i64; // { dg-warning "" "" { target *-*-* } }

    let fail = 0x1_FFFF_FFFFu32; // { dg-warning "" "" { target *-*-* } }

    let fail: i128 = 0x8000_0000_0000_0000_0000_0000_0000_0000;
// { dg-warning "" "" { target *-*-* } .-1 }

    let fail = 0x8FFF_FFFF_FFFF_FFFE; // { dg-warning "" "" { target *-*-* } }

    let fail = -0b1111_1111i8; // { dg-warning "" "" { target *-*-* } }
}

