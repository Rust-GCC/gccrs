// compile-flags: -Z borrowck=mir

#![allow(dead_code)]

use std::fmt::Debug;

fn bar<'a>(x: &'a u32) -> &'static dyn Debug {
    x
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

