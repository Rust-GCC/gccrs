#![feature(box_syntax)]

fn main() {
    let f;
    let g;
    g = f;
    f = box g;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

