// aux-build:const_evaluatable_lib.rs
#![feature(const_generics, const_evaluatable_checked)]
#![allow(incomplete_features)]
extern crate const_evaluatable_lib;

fn user<T>() {
    let _ = const_evaluatable_lib::test1::<T>();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
}

fn main() {}

