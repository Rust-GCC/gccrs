#![deny(trivial_casts, trivial_numeric_casts)]
#![feature(type_ascription)]

fn main() {
    let lugubrious = 12i32 as i32;
// { dg-error "" "" { target *-*-* } .-1 }
    let haunted: &u32 = &99;
    let _ = haunted as *const u32;
// { dg-error "" "" { target *-*-* } .-1 }
}

