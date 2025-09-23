#![feature(never_type)]
#![allow(unused_variables)]
#![allow(unused_assignments)]
#![allow(dead_code)]
#![deny(unreachable_code)]

fn foo() {
    // No error here.
    let x;
    x = return; // { dg-error "" "" { target *-*-* } }
}

fn bar() {
    use std::ptr;
    let p: *mut ! = ptr::null_mut::<!>();
    unsafe {
        // Here we consider the `return` unreachable because
        // "evaluating" the `*p` has type `!`. This is somewhat
        // dubious, I suppose.
        *p = return; // { dg-error "" "" { target *-*-* } }
    }
}

fn baz() {
    let mut i = 0;
    *{return; &mut i} = 22; // { dg-error "" "" { target *-*-* } }
}

fn main() { }

