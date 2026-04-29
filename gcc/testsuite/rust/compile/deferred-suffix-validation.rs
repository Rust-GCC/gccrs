#![feature(no_core)]
#![no_core]

macro_rules! foo {
    ($x:tt) => {};
}

fn main() {
    foo!(123invalid);
    foo!(3.14_f128_invalid);
    foo!(0x1h12);
    foo!(0o1h13);
    foo!(0b1h14);

    let _ = 123invalid; // { dg-error "invalid suffix .invalid. for number literal" }
}
