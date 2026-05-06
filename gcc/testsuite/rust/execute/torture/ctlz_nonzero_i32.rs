#![feature(no_core)]
#![no_core]
#![feature(intrinsics)]
#![feature(lang_items)]

#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    pub fn ctlz_nonzero<T>(x: T) -> u32;
    pub fn abort() -> !;
}

fn main() -> i32 {
    unsafe {
        // 1i32 = 0x00000001: 31 leading zeros
        if ctlz_nonzero(1i32) != 31 {
            abort();
        }
        // -1i32 = 0xFFFFFFFF: 0 leading zeros
        if ctlz_nonzero(-1i32) != 0 {
            abort();
        }
    }

    0
}
