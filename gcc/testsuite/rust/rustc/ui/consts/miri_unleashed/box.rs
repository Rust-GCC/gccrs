// compile-flags: -Zunleash-the-miri-inside-of-you
#![feature(box_syntax)]
#![allow(const_err)]

use std::mem::ManuallyDrop;

fn main() {}

static TEST_BAD: &mut i32 = {
    &mut *(box 0)
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-note ".E0080." "" { target *-*-* } .-2 }
};

