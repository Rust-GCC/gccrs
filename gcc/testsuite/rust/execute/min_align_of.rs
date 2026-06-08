// { dg-require-effective-target lp64 }
#![no_core]
#![feature(no_core, intrinsics, lang_items)]

#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    pub fn min_align_of<T>() -> usize;
}

fn main() -> i32 {
    let align_u16 = get_u16_align() as i32;
    let align_u32 = get_i32_align() as i32;

    if align_u16 != 2 || align_u32 != 4 {
        1
    } else {
        0
    }
}

pub fn get_u16_align() -> usize {
    unsafe { min_align_of::<u16>() }
}

pub fn get_i32_align() -> usize {
    unsafe { min_align_of::<i32>() }
}
