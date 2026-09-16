#![feature(intrinsics)]

extern "C" {
    pub static FOO: extern "rust-intrinsic" fn();
}

fn main() {
    FOO() // { dg-error ".E0133." "" { target *-*-* } }
}

