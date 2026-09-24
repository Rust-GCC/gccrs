// Tests that `transmute` cannot be called on type parameters.

use std::mem::transmute;

unsafe fn f<T>(x: T) {
    let _: i32 = transmute(x);
// { dg-error ".E0512." "" { target *-*-* } .-1 }
}

unsafe fn g<T>(x: (T, i32)) {
    let _: i32 = transmute(x);
// { dg-error ".E0512." "" { target *-*-* } .-1 }
}

unsafe fn h<T>(x: [T; 10]) {
    let _: i32 = transmute(x);
// { dg-error ".E0512." "" { target *-*-* } .-1 }
}

struct Bad<T> {
    f: T,
}

unsafe fn i<T>(x: Bad<T>) {
    let _: i32 = transmute(x);
// { dg-error ".E0512." "" { target *-*-* } .-1 }
}

enum Worse<T> {
    A(T),
    B,
}

unsafe fn j<T>(x: Worse<T>) {
    let _: i32 = transmute(x);
// { dg-error ".E0512." "" { target *-*-* } .-1 }
}

unsafe fn k<T>(x: Option<T>) {
    let _: i32 = transmute(x);
// { dg-error ".E0512." "" { target *-*-* } .-1 }
}

fn main() {}

