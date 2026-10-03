#![allow(dead_code)]

use std::panic::UnwindSafe;
use std::cell::UnsafeCell;

fn assert<T: UnwindSafe + ?Sized>() {}

fn main() {
    assert::<*const UnsafeCell<i32>>(); // { dg-error ".E0277." "" { target *-*-* } }
}

