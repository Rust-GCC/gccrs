#![feature(intrinsics)]
extern "rust-intrinsic" {
    fn foo();
// { dg-error ".E0093." "" { target *-*-* } .-1 }
}

fn main() {
}

