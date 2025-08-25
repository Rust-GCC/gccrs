#![feature(intrinsics)]

extern "rust-intrinsic" {
    fn size_of<T>(); // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {
}

