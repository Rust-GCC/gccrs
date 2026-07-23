// compile-flags: -Zverbose

// Same as test/ui/generator/not-send-sync.rs
#![feature(generators)]

use std::cell::Cell;

fn main() {
    fn assert_sync<T: Sync>(_: T) {}
    fn assert_send<T: Send>(_: T) {}

    assert_sync(|| {
// { dg-error "" "" { target *-*-* } .-1 }
        let a = Cell::new(2);
        yield;
    });

    let a = Cell::new(2);
    assert_send(|| {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        drop(&a);
        yield;
    });
}

