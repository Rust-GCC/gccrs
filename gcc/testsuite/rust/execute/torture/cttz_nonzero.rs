#![feature(no_core)]
#![no_core]
#![feature(intrinsics)]
#![feature(lang_items)]

#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    pub fn cttz_nonzero<T>(x: T) -> u32;
    pub fn abort() -> !;
}

fn main() -> i32 {
    unsafe {
        if cttz_nonzero(1u8) != 0 {
            abort();
        }
        if cttz_nonzero(0xFFu8) != 0 {
            abort();
        }

        if cttz_nonzero(1u16) != 0 {
            abort();
        }
        if cttz_nonzero(0xFFFFu16) != 0 {
            abort();
        }

        if cttz_nonzero(1u32) != 0 {
            abort();
        }
        if cttz_nonzero(0xFFFFFFFFu32) != 0 {
            abort();
        }

        if cttz_nonzero(1u64) != 0 {
            abort();
        }
        if cttz_nonzero(!0u64) != 0 {
            abort();
        }

        if cttz_nonzero(1i8) != 0 {
            abort();
        }
        if cttz_nonzero(-1i8) != 0 {
            abort();
        }

        if cttz_nonzero(1i16) != 0 {
            abort();
        }
        if cttz_nonzero(-1i16) != 0 {
            abort();
        }

        if cttz_nonzero(1i32) != 0 {
            abort();
        }
        if cttz_nonzero(-1i32) != 0 {
            abort();
        }

        if cttz_nonzero(1i64) != 0 {
            abort();
        }
        if cttz_nonzero(-1i64) != 0 {
            abort();
        }
    }

    0
}
