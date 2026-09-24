#![allow(unused_macros)]

#[allow_internal_unstable()] // { dg-error ".E0658." "" { target *-*-* } }
macro_rules! foo {
    () => {}
}

fn main() {}

