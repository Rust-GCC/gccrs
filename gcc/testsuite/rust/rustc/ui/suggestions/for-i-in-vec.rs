// run-rustfix
#![allow(dead_code)]

struct Foo {
    v: Vec<u32>,
}

impl Foo {
    fn bar(&self) {
        for _ in self.v { // { dg-error ".E0507." "" { target *-*-* } }
        }
    }
}

fn main() {}

