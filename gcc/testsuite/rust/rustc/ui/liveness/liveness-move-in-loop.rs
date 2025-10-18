#![feature(box_syntax)]

fn main() {
    let y: Box<isize> = box 42;
    let mut x: Box<isize>;
    loop {
        println!("{}", y);
        loop {
            loop {
                loop {
                    x = y; // { dg-error ".E0382." "" { target *-*-* } }
                    x.clone();
                }
            }
        }
    }
}

