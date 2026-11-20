// Regression test for #75118.

use std::ptr::NonNull;

pub const fn dangling_slice<T>() -> NonNull<[T]> {
    NonNull::<[T; 0]>::dangling()
// { dg-error ".E0723." "" { target *-*-* } .-1 }
}

fn main() {}

