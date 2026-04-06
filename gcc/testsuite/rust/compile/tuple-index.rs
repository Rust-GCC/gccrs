#![feature(no_core)]
#![no_core]

fn main() {
    let t = (10, 20);
    let _a = t.0;
    let _b = t.1;

    struct S(u8, u8);
    let s = S(1, 2);
    let _c = s.0;
}
