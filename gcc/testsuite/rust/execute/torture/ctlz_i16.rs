#![feature(no_core)]
#![no_core]
#![feature(intrinsics)]
#![feature(lang_items)]

#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    pub fn ctlz<T>(x: T) -> u32;
    pub fn abort() -> !;
}

fn main() -> i32 {
    if ctlz(0i16) != 16 {
        abort();
    }
    // 1i16 = 0x0001: 15 leading zeros
    if ctlz(1i16) != 15 {
        abort();
    }
    // -1i16 = 0xFFFF: 0 leading zeros
    if ctlz(-1i16) != 0 {
        abort();
    }

    0
}
