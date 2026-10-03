#![feature(box_syntax)]

fn take(_x: Box<isize>) {}

fn main() {

    let x: Box<isize> = box 25;
    loop {
        take(x); // { dg-error ".E0382." "" { target *-*-* } }
    }
}

