// Verify that UnsafeCell is *always* !Sync regardless if `T` is sync.

#![feature(negative_impls)]

use std::cell::UnsafeCell;
use std::marker::Sync;

struct MySync<T> {
    u: UnsafeCell<T>
}

struct NoSync;
impl !Sync for NoSync {}

fn test<T: Sync>(s: T) {}

fn main() {
    let us = UnsafeCell::new(MySync{u: UnsafeCell::new(0)});
    test(us);
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    let uns = UnsafeCell::new(NoSync);
    test(uns);
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    let ms = MySync{u: uns};
    test(ms);
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    test(NoSync);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

