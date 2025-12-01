#![feature(box_syntax)]

fn main() {
    let f;
    f = box f;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

