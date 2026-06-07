// { dg-additional-options "-frust-c-style-string-literals" }
#![feature(no_core, intrinsics, staged_api, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

// below's helper code copied from issue-1232.rs
extern "rust-intrinsic" {
    #[rustc_const_stable(feature = "const_ptr_offset", since = "1.61.0")]
    fn offset<T>(dst: *const T, offset: isize) -> *const T;
}

#[lang = "const_ptr"]
impl<T> *const T {
    pub const unsafe fn offset(self, count: isize) -> *const T {
        unsafe { offset(self, count) }
    }

    pub const unsafe fn add(self, count: usize) -> Self {
        unsafe { self.offset(count as isize) }
    }

    pub const fn as_ptr(self) -> *const T {
        self as *const T
    }
}

extern "C" {
    fn printf(s: *const u8, ...);
}

type c_char = u8;

#[lang = "CStr"]
pub struct CStr {
    inner: [c_char]
}

impl CStr {
    pub const fn to_ptr(&self) -> *const c_char {
        &self.inner as *const [c_char] as *const c_char
    }
}

pub fn main() -> u8 {
    let a = c"gccrs";
    let val = unsafe { a.to_ptr().add(5) };
    unsafe { *val }
}
