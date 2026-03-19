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
    if ctlz(0i64) != 64 {
        abort();
    }
    // 1i64 = 0x0000000000000001: 63 leading zeros
    if ctlz(1i64) != 63 {
        abort();
    }
    // -1i64 = 0xFFFFFFFFFFFFFFFF: 0 leading zeros
    if ctlz(-1i64) != 0 {
        abort();
    }

    0
}
