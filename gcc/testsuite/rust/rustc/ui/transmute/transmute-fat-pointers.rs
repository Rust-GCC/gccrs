// normalize-stderr-test "\d+ bits" -> "N bits"

// Tests that are conservative around thin/fat pointer mismatches.

#![allow(dead_code)]

use std::mem::transmute;

fn a<T, U: ?Sized>(x: &[T]) -> &U {
    unsafe { transmute(x) } // { dg-error ".E0512." "" { target *-*-* } }
}

fn b<T: ?Sized, U: ?Sized>(x: &T) -> &U {
    unsafe { transmute(x) } // { dg-error ".E0512." "" { target *-*-* } }
}

fn c<T, U>(x: &T) -> &U {
    unsafe { transmute(x) }
}

fn d<T, U>(x: &[T]) -> &[U] {
    unsafe { transmute(x) }
}

fn e<T: ?Sized, U>(x: &T) -> &U {
    unsafe { transmute(x) } // { dg-error ".E0512." "" { target *-*-* } }
}

fn f<T, U: ?Sized>(x: &T) -> &U {
    unsafe { transmute(x) } // { dg-error ".E0512." "" { target *-*-* } }
}

fn main() { }

