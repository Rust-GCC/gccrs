// compile-flags: -Zunleash-the-miri-inside-of-you
#![allow(const_err)]

use std::sync::atomic::AtomicUsize;
use std::sync::atomic::Ordering;

// These only fail during validation (they do not use but just create a reference to a static),
// so they cause an immediate error when *defining* the const.

const REF_INTERIOR_MUT: &usize = { // { dg-error ".E0080." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
    static FOO: AtomicUsize = AtomicUsize::new(0);
    unsafe { &*(&FOO as *const _ as *const usize) }
};

// ok some day perhaps
const READ_IMMUT: &usize = { // { dg-error ".E0080." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
    static FOO: usize = 0;
    &FOO
};

fn main() {}

