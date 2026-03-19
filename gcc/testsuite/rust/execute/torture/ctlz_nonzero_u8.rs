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
        if ctlz_nonzero(1u8) != 7 {
            abort();
        }
        if ctlz_nonzero(255u8) != 0 {
            abort();
        }
    }

    0
}
