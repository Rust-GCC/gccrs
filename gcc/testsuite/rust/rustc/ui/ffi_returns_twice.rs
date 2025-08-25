#![feature(ffi_returns_twice)]
#![crate_type = "lib"]

#[ffi_returns_twice] // { dg-error ".E0724." "" { target *-*-* } }
pub fn foo() {}

