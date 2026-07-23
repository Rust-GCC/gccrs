// compile-flags: --cfg broken

#![crate_type = "lib"]
#![cfg_attr(broken, no_std, no_core)]
// { dg-error ".E0658." "" { target *-*-* } .-1 }

pub struct S {}

