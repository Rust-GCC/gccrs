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
    if ctlz(0u8) != 8 {
        abort();
    }
    if ctlz(1u8) != 7 {
        abort();
    }
    if ctlz(255u8) != 0 {
        abort();
    }

    0
}
