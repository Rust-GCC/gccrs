#![feature(box_syntax)]

static mut a: Box<isize> = box 3;
// { dg-error ".E0010." "" { target *-*-* } .-1 }

fn main() {}

