use std::cell::UnsafeCell;

const A: UnsafeCell<usize> = UnsafeCell::new(1);
const B: &'static UnsafeCell<usize> = &A;
// { dg-error ".E0492." "" { target *-*-* } .-1 }

struct C { a: UnsafeCell<usize> }
const D: C = C { a: UnsafeCell::new(1) };
const E: &'static UnsafeCell<usize> = &D.a;
// { dg-error ".E0492." "" { target *-*-* } .-1 }
const F: &'static C = &D;
// { dg-error ".E0492." "" { target *-*-* } .-1 }

fn main() {}

