#![allow(const_err, invalid_value)] // make sure we cannot allow away the errors tested here

use std::mem;

const BAD_UPVAR: &dyn FnOnce() = &{ // { dg-error ".E0080." "" { target *-*-* } }
    let bad_ref: &'static u16 = unsafe { mem::transmute(0usize) };
    let another_var = 13;
    move || { let _ = bad_ref; let _ = another_var; }
};

fn main() {}

