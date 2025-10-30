// Testing guarantees provided by once functions.
// This program would segfault if it were legal.

#![feature(once_fns)]
use std::sync::Arc;

fn foo<F:FnOnce()>(blk: F) {
    blk();
    blk(); // { dg-error ".E0382." "" { target *-*-* } }
}

fn main() {
    let x = Arc::new(true);
    foo(move|| {
        assert!(*x);
        drop(x);
    });
}

