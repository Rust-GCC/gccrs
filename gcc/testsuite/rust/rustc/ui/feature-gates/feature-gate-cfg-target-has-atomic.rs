#![feature(intrinsics, lang_items, no_core, rustc_attrs)]

#![crate_type="rlib"]
#![no_core]

extern "rust-intrinsic" {
    fn atomic_xadd<T>(dst: *mut T, src: T) -> T;
}

#[lang = "sized"]
trait Sized {}
#[lang = "copy"]
trait Copy {}

#[cfg(target_has_atomic = "8")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_u8(x: *mut u8) {
    atomic_xadd(x, 1);
    atomic_xadd(x, 1);
}
#[cfg(target_has_atomic = "8")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_i8(x: *mut i8) {
    atomic_xadd(x, 1);
}
#[cfg(target_has_atomic = "16")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_u16(x: *mut u16) {
    atomic_xadd(x, 1);
}
#[cfg(target_has_atomic = "16")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_i16(x: *mut i16) {
    atomic_xadd(x, 1);
}
#[cfg(target_has_atomic = "32")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_u32(x: *mut u32) {
    atomic_xadd(x, 1);
}
#[cfg(target_has_atomic = "32")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_i32(x: *mut i32) {
    atomic_xadd(x, 1);
}
#[cfg(target_has_atomic = "64")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_u64(x: *mut u64) {
    atomic_xadd(x, 1);
}
#[cfg(target_has_atomic = "64")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_i64(x: *mut i64) {
    atomic_xadd(x, 1);
}
#[cfg(target_has_atomic = "128")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_u128(x: *mut u128) {
    atomic_xadd(x, 1);
}
#[cfg(target_has_atomic = "128")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_i128(x: *mut i128) {
    atomic_xadd(x, 1);
}
#[cfg(target_has_atomic = "ptr")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_usize(x: *mut usize) {
    atomic_xadd(x, 1);
}
#[cfg(target_has_atomic = "ptr")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
pub unsafe fn atomic_isize(x: *mut isize) {
    atomic_xadd(x, 1);
}

fn main() {
    cfg!(target_has_atomic = "8");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic = "16");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic = "32");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic = "64");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic = "128");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic = "ptr");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_load_store = "8");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_load_store = "16");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_load_store = "32");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_load_store = "64");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_load_store = "128");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_load_store = "ptr");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_equal_alignment = "8");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_equal_alignment = "16");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_equal_alignment = "32");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_equal_alignment = "64");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_equal_alignment = "128");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    cfg!(target_has_atomic_equal_alignment = "ptr");
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

#[macro_export]
#[rustc_builtin_macro]
macro_rules! cfg { () => () }

