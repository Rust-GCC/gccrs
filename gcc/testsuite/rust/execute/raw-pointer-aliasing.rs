// { dg-options "-O2" }

#![feature(no_core)]
#![no_core]

extern "C" {
    fn malloc(n: u64) -> *mut u8;
}

fn f() -> i32 {
    unsafe {
        let p = malloc(4) as *mut i32;
        *p = 27;
        *(p as *mut i16) = 42;
        *p
    }
}

fn main() -> i32 {
    if f() == 27 {
        1
    } else {
        0
    }
}
