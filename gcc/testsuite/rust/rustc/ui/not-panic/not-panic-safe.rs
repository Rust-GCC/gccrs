#![allow(dead_code)]
#![feature(recover)]

use std::panic::UnwindSafe;

fn assert<T: UnwindSafe + ?Sized>() {}

fn main() {
    assert::<&mut i32>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

