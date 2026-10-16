#![allow(warnings)]
#![feature(in_band_lifetimes)]

fn foo(x: &u32) {
    let y: &'test u32 = x; // { dg-error ".E0261." "" { target *-*-* } }
}

fn foo2(x: &u32) {}
fn bar() {
    let y: fn(&'test u32) = foo2; // { dg-error ".E0261." "" { target *-*-* } }
}

fn main() {}

