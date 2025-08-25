#![feature(intrinsics)]
extern "rust-intrinsic" {
    fn size_of<T, U>() -> usize; // { dg-error ".E0094." "" { target *-*-* } }
}

fn main() {
}

