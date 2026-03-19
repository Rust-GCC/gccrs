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
    if ctlz(0u32) != 32 {
        abort();
    }
    if ctlz(1u32) != 31 {
        abort();
    }
    if ctlz(0xFFFFFFFFu32) != 0 {
        abort();
    }

    0
}
