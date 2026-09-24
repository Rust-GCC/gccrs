#![deny(const_err)]

pub const A: i8 = -std::i8::MIN; // { dg-error "" "" { target *-*-* } }
pub const B: u8 = 200u8 + 200u8; // { dg-error "" "" { target *-*-* } }
pub const C: u8 = 200u8 * 4; // { dg-error "" "" { target *-*-* } }
pub const D: u8 = 42u8 - (42u8 + 1); // { dg-error "" "" { target *-*-* } }
pub const E: u8 = [5u8][1]; // { dg-error "" "" { target *-*-* } }

fn main() {
    let _a = A;
    let _b = B;
    let _c = C;
    let _d = D;
    let _e = E;
    let _e = [6u8][1];
}

