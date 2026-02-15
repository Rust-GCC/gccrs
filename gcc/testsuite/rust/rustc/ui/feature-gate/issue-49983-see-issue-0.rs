extern crate core;

// error should not say "(see issue #0)"
#[allow(unused_imports)] use core::ptr::Unique; // { dg-error ".E0658." "" { target *-*-* } }

fn main() {}

