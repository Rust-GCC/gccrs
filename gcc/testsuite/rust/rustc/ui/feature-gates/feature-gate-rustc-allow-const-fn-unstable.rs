#![allow(unused_macros)]

#[rustc_allow_const_fn_unstable()] // { dg-error ".E0658." "" { target *-*-* } }
const fn foo() { }

fn main() {}

