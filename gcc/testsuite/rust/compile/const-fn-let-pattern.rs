// items only used in constants are reported as unused
// { dg-additional-options "-w" }
#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

struct S {
    a: usize,
    b: usize,
}

const fn wildcard() -> usize {
    let s = 4;
    let _ = s;
    s
}

const fn tuple() -> usize {
    let t = (3, 4);
    let (a, b) = t;
    a + b
}

const fn record() -> usize {
    let s = S { a: 5, b: 6 };
    let S { a, b } = s;
    a + b
}

pub fn check() {
    let _w: [u8; wildcard()] = [0; 4];
    let _t: [u8; tuple()] = [0; 7];
    let _r: [u8; record()] = [0; 11];
}
