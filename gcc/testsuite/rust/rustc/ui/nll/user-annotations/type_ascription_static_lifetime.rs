#![allow(warnings)]
#![feature(type_ascription)]

fn main() {
    let x = 22_u32;
    let y: &u32 = &x: &'static u32; // { dg-error ".E0597." "" { target *-*-* } }
}

