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
        // 1i8 = 0x01: 7 leading zeros
        if ctlz_nonzero(1i8) != 7 {
            abort();
        }
        // -1i8 = 0xFF: 0 leading zeros
        if ctlz_nonzero(-1i8) != 0 {
            abort();
        }
    }

    0
}
