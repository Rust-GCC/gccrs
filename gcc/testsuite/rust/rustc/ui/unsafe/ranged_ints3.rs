#![feature(rustc_attrs)]

use std::cell::Cell;

#[rustc_layout_scalar_valid_range_start(1)]
#[repr(transparent)]
pub(crate) struct NonZero<T>(pub(crate) T);
fn main() {
    let mut x = unsafe { NonZero(Cell::new(1)) };
    let y = &x.0; // { dg-error ".E0133." "" { target *-*-* } }
}

