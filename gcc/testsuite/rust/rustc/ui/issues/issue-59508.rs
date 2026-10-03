// run-rustfix

#![allow(dead_code)]

// This test checks that generic parameter re-ordering diagnostic suggestions contain bounds.

struct A;

impl A {
    pub fn do_things<T, 'a, 'b: 'a>() {
// { dg-error "" "" { target *-*-* } .-1 }
        println!("panic");
    }
}

fn main() {}

