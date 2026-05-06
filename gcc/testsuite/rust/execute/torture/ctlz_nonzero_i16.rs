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
        // 1i16 = 0x0001: 15 leading zeros
        if ctlz_nonzero(1i16) != 15 {
            abort();
        }
        // -1i16 = 0xFFFF: 0 leading zeros
        if ctlz_nonzero(-1i16) != 0 {
            abort();
        }
    }

    0
}
