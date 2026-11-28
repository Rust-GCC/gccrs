#![feature(box_syntax)]

fn dup(x: Box<isize>) -> Box<(Box<isize>,Box<isize>)> {
    box (x, x)
// { dg-error ".E0382." "" { target *-*-* } .-1 }
}

fn main() {
    dup(box 3);
}

