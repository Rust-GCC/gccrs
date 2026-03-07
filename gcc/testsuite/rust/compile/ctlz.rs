// { dg-do compile }
#![feature(intrinsics, lang_items, no_core)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

#[lang = "copy"]
pub trait Copy {}

extern "rust-intrinsic" {
    pub fn ctlz<T>(x: T) -> u32; // { dg-error "ctlz intrinsics can only be used with basic integer types .got 'bool'." }
}

fn main() {
    let _ = ctlz(true);
}
