//! Regression test for https://github.com/rust-lang/rust/issues/62211
//!
//! The old implementation of defaults did not check whether the provided
//! default actually fulfills all bounds on the assoc. type, leading to
//! unsoundness and ICEs, the latter being demonstrated here.
//!
//! Note that the underlying cause of this is still not yet fixed.
//! See: https://github.com/rust-lang/rust/issues/33017

#![feature(associated_type_defaults)]

use std::{
    fmt::Display,
    ops::{AddAssign, Deref},
};

trait UncheckedCopy: Sized {
    // This Output is said to be Copy. Yet we default to Self
    // and it's accepted, not knowing if Self ineed is Copy
    type Output: Copy + Deref<Target = str> + AddAssign<&'static str> + From<Self> + Display = Self;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
// { dg-error ".E0277." "" { target *-*-* } .-4 }

    // We said the Output type was Copy, so we can Copy it freely!
    fn unchecked_copy(other: &Self::Output) -> Self::Output {
        (*other)
    }

    fn make_origin(s: Self) -> Self::Output {
        s.into()
    }
}

impl<T> UncheckedCopy for T {}

fn bug<T: UncheckedCopy>(origin: T) {
    let origin = T::make_origin(origin);
    let mut copy = T::unchecked_copy(&origin);

    // assert we indeed have 2 strings pointing to the same buffer.
    assert_eq!(origin.as_ptr(), copy.as_ptr());

    // Drop the origin. Any use of `copy` is UB.
    drop(origin);

    copy += "This is invalid!";
    println!("{}", copy);
}

fn main() {
    bug(());
}

