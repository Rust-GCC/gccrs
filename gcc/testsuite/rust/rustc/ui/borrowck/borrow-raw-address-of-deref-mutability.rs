// Check that `&raw mut` cannot be used to turn a `&T` into a `*mut T`.

#![feature(raw_ref_op)]

fn raw_reborrow() {
    let x = &0;

    let q = &raw mut *x;                // { dg-error ".E0596." "" { target *-*-* } }
}

unsafe fn raw_reborrow_of_raw() {
    let x = &0 as *const i32;

    let q = &raw mut *x;                // { dg-error ".E0596." "" { target *-*-* } }
}

fn main() {}

