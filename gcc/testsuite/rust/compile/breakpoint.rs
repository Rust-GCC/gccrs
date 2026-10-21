#![feature(no_core)]
#![feature(intrinsics)]
#![no_core]

extern "rust-intrinsic" {
    fn breakpoint();
}

enum Nothing {}

fn foo() {
    unsafe { breakpoint() };
}

fn main() {
    foo();
}
