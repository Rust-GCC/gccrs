#![feature(box_syntax)]

fn main() {
    let x: Box<_> = box 1;
    let f = move|| {
        let _a = x;
        drop(x);
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    };
    f();
}

