#![feature(rustc_private)]

extern crate alloc;

extern crate libc as alloc;
// { dg-error ".E0259." "" { target *-*-* } .-1 }

fn main() {}

