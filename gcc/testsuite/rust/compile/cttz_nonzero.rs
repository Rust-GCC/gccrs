// { dg-do compile }
#![feature(intrinsics, lang_items, no_core)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

#[lang = "copy"]
pub trait Copy {}

extern "rust-intrinsic" {
    pub fn cttz_nonzero<T>(x: T) -> u32; // { dg-error "cttz intrinsics can only be used with basic integer types .got 'bool'." }
}

fn main() {
    unsafe {
        let _ = cttz_nonzero(true);
    }
}
