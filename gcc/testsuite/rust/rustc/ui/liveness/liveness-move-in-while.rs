#![feature(box_syntax)]

fn main() {
    let y: Box<isize> = box 42;
    let mut x: Box<isize>;
    loop {
        println!("{}", y); // { dg-error ".E0382." "" { target *-*-* } }
        while true { while true { while true { x = y; x.clone(); } } }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
    }
}

