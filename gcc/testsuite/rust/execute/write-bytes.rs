#![feature(no_core, intrinsics, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    fn write_bytes<T>(dst: *mut T, val: u8, count: usize);
}

fn main() -> i32 {
    let mut x: u32 = 0;
    unsafe { 
        write_bytes(&mut x as *mut u32, 0xEC_u8, 1); 
    }
    if x == 0xECECECEC_u32 { 0 } else { 1 }
}
