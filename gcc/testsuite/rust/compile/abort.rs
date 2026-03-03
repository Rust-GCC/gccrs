#![feature(intrinsics, no_core)]
#![no_core]

extern "rust-intrinsic" {
    fn abort() -> !;
}

fn main() {
    unsafe {
        abort();
    }
}
// { dg-final { scan-assembler "abort" } }
