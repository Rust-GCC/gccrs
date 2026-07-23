// aux-build:two_macros.rs

#![feature(macro_reexport)] // { dg-error ".E0557." "" { target *-*-* } }

#[macro_reexport(macro_one)] // { dg-error "" "" { target *-*-* } }
extern crate two_macros;

fn main() {}

