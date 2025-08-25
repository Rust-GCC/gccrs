#![deny(const_err)]

pub const A: i8 = -std::i8::MIN;
// { dg-error "" "" { target *-*-* } .-1 }
pub const B: i8 = A;
// { dg-error "" "" { target *-*-* } .-1 }
pub const C: u8 = A as u8;
// { dg-error "" "" { target *-*-* } .-1 }
pub const D: i8 = 50 - A;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    let _ = (A, B, C, D);
}

