#![feature(no_core, rustc_attrs)]
#![no_core]

#[rustc_allocator] // { dg-error ".rustc_allocator. attribute must be applied to a function returning a pointer" }
pub fn _foo() {}

