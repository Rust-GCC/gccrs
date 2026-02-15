// Test sized-ness checking in substitution within fn bodies..

use std::marker;

// Unbounded.
fn f1<X: ?Sized>(x: &X) {
    f2::<X>(x);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}
fn f2<X>(x: &X) {
}

// Bounded.
trait T {
    fn foo(&self) { }
}
fn f3<X: ?Sized + T>(x: &X) {
    f4::<X>(x);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}
fn f4<X: T>(x: &X) {
}

fn f5<Y>(x: &Y) {}
fn f6<X: ?Sized>(x: &X) {}

// Test with unsized struct.
struct S<X: ?Sized> {
    x: X,
}

fn f8<X: ?Sized>(x1: &S<X>, x2: &S<X>) {
    f5(x1);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    f6(x2); // ok
}

// Test some tuples.
fn f9<X: ?Sized>(x1: Box<S<X>>) {
    f5(&(*x1, 34));
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn f10<X: ?Sized>(x1: Box<S<X>>) {
    f5(&(32, *x1));
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

pub fn main() {
}

