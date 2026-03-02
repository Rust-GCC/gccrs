// { dg-do run }
// { dg-options "-O2" }
#![feature(intrinsics, lang_items, no_core)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}
#[lang = "copy"]
pub trait Copy {}

extern "rust-intrinsic" {
    pub fn black_box<T>(dummy: T) -> T;
}

extern "C" {
    fn abort();
}

struct MyStruct {
    a: i32,
    b: i64,
}

pub extern "C" fn main() -> i32 {

    let x = 12345;
    let y = unsafe { black_box(x) };

    if y != 12345 {
        unsafe { abort(); }
    }

    let s1 = MyStruct { a: 10, b: 20 };
    let s2 = unsafe { black_box(s1) };

    if s2.a != 10 || s2.b != 20 {
        unsafe { abort(); }
    }

    0
}