#![feature(box_syntax)]

fn main() {
    let x: Box<_> = box 5;
    let y = x;
    println!("{}", *x); // { dg-error ".E0382." "" { target *-*-* } }
    y.clone();
}

