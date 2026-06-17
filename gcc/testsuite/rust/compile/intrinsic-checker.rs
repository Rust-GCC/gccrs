#![feature(no_core, intrinsics, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    fn foo(); // { dg-error "unrecognized intrinsic function: .foo." }

    fn size_of<T, U>() -> usize; // { dg-error "intrinsic has wrong number of type parameters: found 2, expected 1" }

    fn abort(x: i32) -> !; // { dg-error "intrinsic has wrong number of function parameters: found 1, expected 0" }

    fn offset<T>(dst: usize, offset: isize) -> *const T;
    // { dg-error "intrinsic has wrong type" "" { target *-*-* } .-1 }
    // { dg-message "expected fn pointer .extern \"rust-intrinsic\" fn\\(\\*const P0, isize\\) -> \\*const P0." "" { target *-*-* } .-2 }
    // { dg-message "found fn pointer .extern \"rust-intrinsic\" fn\\(usize, isize\\) -> \\*const P0." "" { target *-*-* } .-3 }

    fn add_with_overflow<T>(x: T, y: i32) -> (T, bool);
    // { dg-error "intrinsic has wrong type" "" { target *-*-* } .-1 }

    fn catch_unwind(try_fn: usize, data: *mut u8, catch_fn: fn(*mut u8, *mut u8)) -> i32;
    // { dg-error "intrinsic has wrong type" "" { target *-*-* } .-1 }

    fn transmute<T, U>(e: T) -> usize;
    // { dg-error "intrinsic has wrong type" "" { target *-*-* } .-1 }
}
