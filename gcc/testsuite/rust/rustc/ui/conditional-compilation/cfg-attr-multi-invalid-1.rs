// compile-flags: --cfg broken

#![crate_type = "lib"]
#![cfg_attr(broken, no_core, no_std)]
// { dg-error ".E0658." "" { target *-*-* } .-1 }

pub struct S {}

