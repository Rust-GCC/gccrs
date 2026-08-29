// compile-flags: -Z teach

#![feature(box_syntax)]
#![allow(warnings)]

const CON : Box<i32> = box 0; // { dg-error ".E0010." "" { target *-*-* } }

fn main() {}

