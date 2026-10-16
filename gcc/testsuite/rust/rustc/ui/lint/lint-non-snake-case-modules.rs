#![deny(non_snake_case)]
#![allow(dead_code)]

mod FooBar { // { dg-error "" "" { target *-*-* } }
    pub struct S;
}

fn f(_: FooBar::S) { }

fn main() { }

