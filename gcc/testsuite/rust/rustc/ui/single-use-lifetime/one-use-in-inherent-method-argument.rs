#![deny(single_use_lifetimes)]
#![allow(dead_code)]
#![allow(unused_variables)]

// Test that we DO warn for a lifetime used only once in an inherent method.

struct Foo<'f> {
    data: &'f u32
}

impl<'f> Foo<'f> { // { dg-error "" "" { target *-*-* } }
    fn inherent_a<'a>(&self, data: &'a u32) { // { dg-error "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-1 }
    }
}

fn main() { }

