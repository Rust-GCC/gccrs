#![feature(ffi_pure)]
#![crate_type = "lib"]

#[ffi_pure] // { dg-error ".E0755." "" { target *-*-* } }
pub fn foo() {}

