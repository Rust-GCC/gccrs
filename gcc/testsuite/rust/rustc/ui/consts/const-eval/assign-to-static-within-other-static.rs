// New test for #53818: modifying static memory at compile-time is not allowed.
// The test should never compile successfully

#![feature(const_raw_ptr_deref)]

use std::cell::UnsafeCell;

static mut FOO: u32 = 42;
static BOO: () = unsafe {
    FOO = 5;
// { dg-error ".E0080." "" { target *-*-* } .-1 }
};

fn main() {}

