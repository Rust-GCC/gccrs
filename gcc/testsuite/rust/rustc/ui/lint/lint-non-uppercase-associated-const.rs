#![deny(non_upper_case_globals)]
#![allow(dead_code)]

struct Foo;

impl Foo {
    const not_upper: bool = true;
}
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

