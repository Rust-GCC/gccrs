#![feature(rustc_attrs)]

use std::cell::Cell;

#[rustc_layout_scalar_valid_range_start(1)]
#[repr(transparent)]
pub(crate) struct NonZero<T>(pub(crate) T);
fn main() {}

const fn foo() -> NonZero<Cell<u32>> {
    let mut x = unsafe { NonZero(Cell::new(1)) };
    let y = &x.0; // { dg-error ".E0133." "" { target *-*-* } }
// { dg-error ".E0133." "" { target *-*-* } .-1 }
    unsafe { NonZero(Cell::new(1)) }
}

const fn bar() -> NonZero<Cell<u32>> {
    let mut x = unsafe { NonZero(Cell::new(1)) };
    let y = unsafe { &x.0 }; // { dg-error ".E0492." "" { target *-*-* } }
    unsafe { NonZero(Cell::new(1)) }
}

