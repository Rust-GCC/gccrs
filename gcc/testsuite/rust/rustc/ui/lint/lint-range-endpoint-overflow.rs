#![deny(overflowing_literals)]

fn main() {
    let range_a = 0..256; // { dg-error "" "" { target *-*-* } }
    let range_b = 0..=255; // ok
    let range_c = 0..=256; // { dg-error "" "" { target *-*-* } }
    let range_d = 256..5; // { dg-error "" "" { target *-*-* } }
    let range_e = 0..257; // { dg-error "" "" { target *-*-* } }
    let _range_f = 0..256u8;  // { dg-error "" "" { target *-*-* } }
    let _range_g = 0..128i8;  // { dg-error "" "" { target *-*-* } }

    range_a.collect::<Vec<u8>>();
    range_b.collect::<Vec<u8>>();
    range_c.collect::<Vec<u8>>();
    range_d.collect::<Vec<u8>>();
    range_e.collect::<Vec<u8>>();
}

