#![feature(ffi_const)]
#![crate_type = "lib"]

#[ffi_const] // { dg-error ".E0756." "" { target *-*-* } }
pub fn foo() {}

