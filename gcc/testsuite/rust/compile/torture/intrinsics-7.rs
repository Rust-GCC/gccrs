#![feature(no_core)]
#![no_core]

#![feature(intrinsics)]

#![feature(lang_items)]
#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    pub fn unchecked_add<T>(x: T, y: T) -> T;
}

fn main() {
    struct NotAdd;

    unsafe { unchecked_add(NotAdd, NotAdd) };
    // { dg-error "unchecked operation intrinsic can only be used with basic integer types .got .NotAdd.." "" { target *-*-* } .-1 }
}
