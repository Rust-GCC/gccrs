#![feature(intrinsics)]
extern "rust-intrinsic" {
    fn atomic_foo(); // { dg-error ".E0092." "" { target *-*-* } }
}

fn main() {
}

