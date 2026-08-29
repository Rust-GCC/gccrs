#![no_std]

#[macro_use(foo="bar")]  // { dg-error ".E0466." "" { target *-*-* } }
extern crate std;

fn main() {}

