// Issue #8380


use std::sync::atomic::*;
use std::ptr;

fn main() {
    let x = AtomicBool::new(false);
    let x = *&x; // { dg-error ".E0507." "" { target *-*-* } }
    let x = AtomicIsize::new(0);
    let x = *&x; // { dg-error ".E0507." "" { target *-*-* } }
    let x = AtomicUsize::new(0);
    let x = *&x; // { dg-error ".E0507." "" { target *-*-* } }
    let x: AtomicPtr<usize> = AtomicPtr::new(ptr::null_mut());
    let x = *&x; // { dg-error ".E0507." "" { target *-*-* } }
}

