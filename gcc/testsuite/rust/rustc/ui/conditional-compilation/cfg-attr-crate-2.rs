// https://github.com/rust-lang/rust/issues/21833#issuecomment-72353044

// compile-flags: --cfg broken

#![crate_type = "lib"]
#![cfg_attr(broken, no_core)] // { dg-error ".E0658." "" { target *-*-* } }

pub struct S {}

