#![feature(core_intrinsics)]

use std::intrinsics;

// `move_val_init` has an odd desugaring, check that it is still treated
// as unsafe.
fn main() {
    intrinsics::move_val_init(1 as *mut u32, 1);
// { dg-error ".E0133." "" { target *-*-* } .-1 }
}

