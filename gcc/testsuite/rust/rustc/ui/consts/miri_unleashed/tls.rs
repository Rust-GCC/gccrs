// compile-flags: -Zunleash-the-miri-inside-of-you
#![feature(thread_local)]
#![allow(const_err)]

use std::thread;

#[thread_local]
static A: u8 = 0;

// Make sure we catch accessing thread-local storage.
static TEST_BAD: () = {
    unsafe { let _val = A; }
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-note ".E0080." "" { target *-*-* } .-2 }
};

// Make sure we catch taking a reference to thread-local storage.
static TEST_BAD_REF: () = {
    unsafe { let _val = &A; }
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-note ".E0080." "" { target *-*-* } .-2 }
};

fn main() {}

