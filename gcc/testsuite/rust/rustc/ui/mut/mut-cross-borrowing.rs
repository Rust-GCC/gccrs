#![feature(box_syntax)]

fn f(_: &mut isize) {}

fn main() {
    let mut x: Box<_> = box 3;
    f(x)    // { dg-error ".E0308." "" { target *-*-* } }
}

