#![allow(warnings)]
#![feature(in_band_lifetimes)]

fn foo<'a>(x: &'a u32, y: &'b u32) {} // { dg-error ".E0688." "" { target *-*-* } }

struct Foo<'a> { x: &'a u32 }

impl Foo<'a> {
    fn bar<'b>(x: &'a u32, y: &'b u32, z: &'c u32) {} // { dg-error ".E0688." "" { target *-*-* } }
}

impl<'b> Foo<'a> { // { dg-error ".E0688." "" { target *-*-* } }
    fn baz() {}
}

fn main() {}

