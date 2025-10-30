// run-rustfix

#![allow(dead_code)]

#[repr(align=8)] // { dg-error ".E0693." "" { target *-*-* } }
// { dg-error ".E0693." "" { target *-*-* } .-2 }
struct A(u64);

#[repr(align="8")] // { dg-error ".E0693." "" { target *-*-* } }
// { dg-error ".E0693." "" { target *-*-* } .-2 }
struct B(u64);

fn main() {}

