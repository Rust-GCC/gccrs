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
    // 0i8 has all 8 bits zero
    if ctlz(0i8) != 8 {
        abort();
    }
    // 1i8 = 0x01: 7 leading zeros
    if ctlz(1i8) != 7 {
        abort();
    }
    // -1i8 = 0xFF in two's complement: all bits set, 0 leading zeros
    if ctlz(-1i8) != 0 {
        abort();
    }

    0
}
