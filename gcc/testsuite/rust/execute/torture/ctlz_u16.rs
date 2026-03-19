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
    if ctlz(0u16) != 16 {
        abort();
    }
    if ctlz(1u16) != 15 {
        abort();
    }
    if ctlz(0xFFFFu16) != 0 {
        abort();
    }

    0
}
