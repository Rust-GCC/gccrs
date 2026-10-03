#![feature(min_const_generics)]

use std::mem::size_of;

fn test<const N: usize>() {}

fn ok<const M: usize>() -> [u8; M] {
    [0; { M }]
}

struct Break0<const N: usize>([u8; { N + 1 }]);
// { dg-error "" "" { target *-*-* } .-1 }

struct Break1<const N: usize>([u8; { { N } }]);
// { dg-error "" "" { target *-*-* } .-1 }

fn break2<const N: usize>() {
    let _: [u8; N + 1];
// { dg-error "" "" { target *-*-* } .-1 }
}

fn break3<const N: usize>() {
    let _ = [0; N + 1];
// { dg-error "" "" { target *-*-* } .-1 }
}

struct BreakTy0<T>(T, [u8; { size_of::<*mut T>() }]);
// { dg-error "" "" { target *-*-* } .-1 }

struct BreakTy1<T>(T, [u8; { { size_of::<*mut T>() } }]);
// { dg-error "" "" { target *-*-* } .-1 }

fn break_ty2<T>() {
    let _: [u8; size_of::<*mut T>() + 1];
// { dg-error "" "" { target *-*-* } .-1 }
}

fn break_ty3<T>() {
    let _ = [0; size_of::<*mut T>() + 1];
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}


trait Foo {
    const ASSOC: usize;
}

fn main() {}

