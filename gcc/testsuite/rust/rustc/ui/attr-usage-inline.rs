#![allow(dead_code)]

#[inline]
fn f() {}

#[inline] // { dg-error ".E0518." "" { target *-*-* } }
struct S;

fn main() {}

