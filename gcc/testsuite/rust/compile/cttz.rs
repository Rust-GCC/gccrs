// { dg-do compile }
#![feature(intrinsics, lang_items, no_core)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

#[lang = "copy"]
pub trait Copy {}

extern "rust-intrinsic" {
    pub fn cttz<T>(x: T) -> u32; // { dg-error "cttz intrinsics can only be used with basic integer types .got 'bool'." }
}

fn main() {
    let _ = cttz(true);
}
