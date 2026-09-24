#![allow(warnings)]

fn main() {
    let x = 22_u32;
    let y: &u32 = (&x) as &'static u32; // { dg-error ".E0597." "" { target *-*-* } }
}

