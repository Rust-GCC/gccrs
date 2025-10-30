#![feature(generators)]

// normalize-stderr-test "std::pin::Unpin" -> "std::marker::Unpin"

use std::marker::Unpin;

fn assert_unpin<T: Unpin>(_: T) {
}

fn main() {
    let mut generator = static || {
        yield;
    };
    assert_unpin(generator); // { dg-error ".E0277." "" { target *-*-* } }
}

