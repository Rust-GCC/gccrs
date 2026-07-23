// check-fail

#![feature(intrinsics)]

fn a() {
    let _: unsafe extern "rust-intrinsic" fn(isize) -> usize = std::mem::transmute;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn b() {
    let _ = std::mem::transmute as unsafe extern "rust-intrinsic" fn(isize) -> usize;
// { dg-error ".E0606." "" { target *-*-* } .-1 }
}

fn main() {}

