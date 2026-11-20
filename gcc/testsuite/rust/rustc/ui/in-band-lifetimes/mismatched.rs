#![allow(warnings)]
#![feature(in_band_lifetimes)]

fn foo(x: &'a u32, y: &u32) -> &'a u32 { y } // { dg-error ".E0621." "" { target *-*-* } }

fn foo2(x: &'a u32, y: &'b u32) -> &'a u32 { y } // { dg-error ".E0623." "" { target *-*-* } }

fn main() {}

