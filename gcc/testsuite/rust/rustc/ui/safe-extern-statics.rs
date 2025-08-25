// aux-build:extern-statics.rs

extern crate extern_statics;
use extern_statics::*;

extern {
    static A: u8;
}

fn main() {
    let a = A; // { dg-error ".E0133." "" { target *-*-* } }
    let ra = &A; // { dg-error ".E0133." "" { target *-*-* } }
    let xa = XA; // { dg-error ".E0133." "" { target *-*-* } }
    let xra = &XA; // { dg-error ".E0133." "" { target *-*-* } }
}

