// { dg-additional-options "-frust-c-style-string-literals" }
// { dg-output "gccrs\n" }
#![feature(no_core)]
#![no_core]

extern "C" {
    fn printf(s: *const i8, ...);
}

pub fn main() {
    let a = c"gccrs";
    unsafe {
        printf(a as *const [u8] as *const i8); // TODO change `as *const [u8]` to `.as_ptr()` when C strings are compiled to their own CStr type
    }
}
