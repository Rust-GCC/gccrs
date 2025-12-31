#![allow(warnings)]
#![feature(in_band_lifetimes)]

fn foo(x: fn(&'a u32)) {} // { dg-error ".E0687." "" { target *-*-* } }

fn bar(x: &Fn(&'a u32)) {} // { dg-error ".E0687." "" { target *-*-* } }

fn baz(x: fn(&'a u32), y: &'a u32) {} // { dg-error ".E0687." "" { target *-*-* } }

struct Foo<'a> { x: &'a u32 }

impl Foo<'a> {
    fn bar(&self, x: fn(&'a u32)) {} // { dg-error ".E0687." "" { target *-*-* } }
}

fn main() {}

