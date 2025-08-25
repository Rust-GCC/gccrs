#![allow(non_camel_case_types)]

extern crate alloc;

mod foo {
    pub trait alloc {
        fn do_something();
    }
}

use foo::alloc;
// { dg-error ".E0254." "" { target *-*-* } .-1 }

fn main() {}

