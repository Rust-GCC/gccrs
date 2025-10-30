#![feature(rustc_attrs)]

#[rustc_layout_scalar_valid_range_start(1)]
#[repr(transparent)]
pub(crate) struct NonZero<T>(pub(crate) T);
fn main() {}

const fn foo() -> NonZero<u32> {
    let mut x = unsafe { NonZero(1) };
    x.0 = 0;
// { dg-error ".E0133." "" { target *-*-* } .-1 }
    x
}

const fn bar() -> NonZero<u32> {
    let mut x = unsafe { NonZero(1) };
    unsafe { x.0 = 0 }; // this is UB
    x
}

