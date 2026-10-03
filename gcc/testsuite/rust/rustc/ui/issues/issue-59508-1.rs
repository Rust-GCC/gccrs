#![allow(dead_code)]
#![feature(const_generics)]
// { dg-warning "" "" { target *-*-* } .-1 }

// This test checks that generic parameter re-ordering diagnostic suggestions mention that
// consts come after types and lifetimes when the `const_generics` feature is enabled.
// We cannot run rustfix on this test because of the above const generics warning.

struct A;

impl A {
    pub fn do_things<T, 'a, 'b: 'a>() {
// { dg-error "" "" { target *-*-* } .-1 }
        println!("panic");
    }
}

fn main() {}

