#![feature(intrinsics)]
extern "rust-intrinsic" {
    pub static breakpoint : unsafe extern "rust-intrinsic" fn();
// { dg-error ".E0622." "" { target *-*-* } .-1 }
}
fn main() { unsafe { breakpoint(); } }

