// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core)]
#![no_core]

pub const _: i32 = 0;
// { dg-warning "visibility qualifier on a .const _. item is unused" "" { target *-*-* } .-1 }
