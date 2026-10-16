#![feature(const_raw_ptr_deref)]

use std::{mem, usize};

// Make sure we error with the right kind of error on a too large slice.
const TEST: () = { unsafe { // { dg-note "" "" { target *-*-* } }
    let slice: *const [u8] = mem::transmute((1usize, usize::MAX));
    let _val = &*slice; // { dg-error "" "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
} };

fn main() {}

