// We are keeping this test in case we decide to allow mutable references in statics again
#![feature(const_mut_refs)]
#![allow(const_err)]

static OH_NO: &mut i32 = &mut 42;
// { dg-error ".E0764." "" { target *-*-* } .-1 }
fn main() {
    assert_eq!(*OH_NO, 42);
}

