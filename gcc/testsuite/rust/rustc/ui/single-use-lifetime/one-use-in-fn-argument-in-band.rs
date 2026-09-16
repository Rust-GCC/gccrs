// run-rustfix

#![feature(in_band_lifetimes)]
#![deny(single_use_lifetimes)]
#![allow(dead_code)]
#![allow(unused_variables)]

// Test that we DO warn when lifetime name is used only
// once in a fn argument, even with in band lifetimes.

fn a(x: &'a u32, y: &'b u32) {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
// { help "" "" { target *-*-* } .-4 }
}

fn main() { }

