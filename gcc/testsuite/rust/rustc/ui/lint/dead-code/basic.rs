#![deny(dead_code)]
#![allow(unreachable_code)]

fn foo() { // { dg-error "" "" { target *-*-* } }

    // none of these should have any dead_code exposed to the user
    panic!();

    panic!("foo");

    panic!("bar {}", "baz")
}


fn main() {}

