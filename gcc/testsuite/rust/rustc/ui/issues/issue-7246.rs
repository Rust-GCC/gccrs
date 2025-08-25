#![deny(unreachable_code)]
#![allow(dead_code)]

use std::ptr;
pub unsafe fn g() {
    return;
    if *ptr::null() {}; // { dg-error "" "" { target *-*-* } }
}

pub fn main() {}

