#![allow(dead_code)]

use std::panic::UnwindSafe;
use std::sync::Arc;
use std::cell::RefCell;

fn assert<T: UnwindSafe + ?Sized>() {}

fn main() {
    assert::<Arc<RefCell<i32>>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

