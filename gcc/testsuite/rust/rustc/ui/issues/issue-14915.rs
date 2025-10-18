#![feature(box_syntax)]

fn main() {
    let x: Box<isize> = box 0;

    println!("{}", x + 1);
// { dg-error ".E0369." "" { target *-*-* } .-1 }
}

