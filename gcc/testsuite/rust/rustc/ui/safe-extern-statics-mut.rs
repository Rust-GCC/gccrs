// aux-build:extern-statics.rs

extern crate extern_statics;
use extern_statics::*;

extern {
    static mut B: u8;
}

fn main() {
    let b = B; // { dg-error ".E0133." "" { target *-*-* } }
    let rb = &B; // { dg-error ".E0133." "" { target *-*-* } }
    let xb = XB; // { dg-error ".E0133." "" { target *-*-* } }
    let xrb = &XB; // { dg-error ".E0133." "" { target *-*-* } }
}

