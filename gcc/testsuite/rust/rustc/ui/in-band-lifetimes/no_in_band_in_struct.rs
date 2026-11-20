#![allow(warnings)]
#![feature(in_band_lifetimes)]

struct Foo {
    x: &'test u32, // { dg-error ".E0261." "" { target *-*-* } }
}

enum Bar {
    Baz(&'test u32), // { dg-error ".E0261." "" { target *-*-* } }
}

fn main() {}

