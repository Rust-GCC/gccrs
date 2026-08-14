// compile-flags: -Zunleash-the-miri-inside-of-you
#![feature(core_intrinsics)]
#![allow(const_err)]

// During CTFE, we prevent pointer comparison and pointer-to-int casts.

static CMP: () = {
    let x = &0 as *const _;
    let _v = x == x;
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-note ".E0080." "" { target *-*-* } .-2 }
};

static INT_PTR_ARITH: () = unsafe {
    let x: usize = std::mem::transmute(&0);
    let _v = x + 0;
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-note ".E0080." "" { target *-*-* } .-2 }
};

fn main() {}

