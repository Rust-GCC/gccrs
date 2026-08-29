#![crate_type = "rlib"]

#![no_core] // { dg-error ".E0658." "" { target *-*-* } }

pub struct S {}

