#![allow(warnings)]
#![feature(in_band_lifetimes)]

fn bar<F>(x: &F) where F: Fn(&'a u32) {} // { dg-error ".E0687." "" { target *-*-* } }

fn baz(x: &impl Fn(&'a u32)) {} // { dg-error ".E0687." "" { target *-*-* } }

fn main() {}

