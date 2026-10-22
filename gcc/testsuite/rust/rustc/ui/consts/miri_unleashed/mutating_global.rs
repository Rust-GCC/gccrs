// compile-flags: -Zunleash-the-miri-inside-of-you
#![allow(const_err)]

// Make sure we cannot mutate globals.

static mut GLOBAL: i32 = 0;

static MUTATING_GLOBAL: () = {
    unsafe {
        GLOBAL = 99
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-note ".E0080." "" { target *-*-* } .-2 }
    }
};

fn main() {}

