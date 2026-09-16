#![feature(const_transmute)]

use std::mem;

fn main() {
    let x: &'static u32 = unsafe { &mem::transmute(3.0f32) };
// { dg-error ".E0716." "" { target *-*-* } .-1 }
}

