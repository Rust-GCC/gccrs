// run-rustfix

#[allow(unused)]
use std::fmt::Debug;
// Rustfix should add this, or use `std::fmt::Debug` instead.

#[allow(dead_code)]
fn test_impl(t: impl Sized) {
    println!("{:?}", t);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

#[allow(dead_code)]
fn test_no_bounds<T>(t: T) {
    println!("{:?}", t);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

#[allow(dead_code)]
fn test_one_bound<T: Sized>(t: T) {
    println!("{:?}", t);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

#[allow(dead_code)]
fn test_no_bounds_where<X, Y>(x: X, y: Y) where X: std::fmt::Debug, {
    println!("{:?} {:?}", x, y);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

#[allow(dead_code)]
fn test_one_bound_where<X>(x: X) where X: Sized {
    println!("{:?}", x);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

#[allow(dead_code)]
fn test_many_bounds_where<X>(x: X) where X: Sized, X: Sized {
    println!("{:?}", x);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

pub fn main() { }

