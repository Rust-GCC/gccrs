// ignore-tidy-linelength
//! Test the part of RFC 2514 that is about not applying `DerefMut` coercions
//! of union fields.
#![feature(untagged_unions)]

use std::mem::ManuallyDrop;

union U1<T> { x:(), f: ManuallyDrop<(T,)> }

union U2<T> { x:(), f: (ManuallyDrop<(T,)>,) }

fn main() {
    let mut u : U1<Vec<i32>> = U1 { x: () };
    unsafe { (*u.f).0 = Vec::new() }; // explicit deref, this compiles
    unsafe { u.f.0 = Vec::new() }; // { dg-error "" "" { target *-*-* } }
    unsafe { &mut (*u.f).0 }; // explicit deref, this compiles
    unsafe { &mut u.f.0 }; // { dg-error "" "" { target *-*-* } }
    unsafe { (*u.f).0.push(0) }; // explicit deref, this compiles
    unsafe { u.f.0.push(0) }; // { dg-error "" "" { target *-*-* } }

    let mut u : U2<Vec<i32>> = U2 { x: () };
    unsafe { (*u.f.0).0 = Vec::new() }; // explicit deref, this compiles
    unsafe { u.f.0.0 = Vec::new() }; // { dg-error "" "" { target *-*-* } }
    unsafe { &mut (*u.f.0).0 }; // explicit deref, this compiles
    unsafe { &mut u.f.0.0 }; // { dg-error "" "" { target *-*-* } }
    unsafe { (*u.f.0).0.push(0) }; // explicit deref, this compiles
    unsafe { u.f.0.0.push(0) }; // { dg-error "" "" { target *-*-* } }
}

