#![deny(non_snake_case)]
#![allow(dead_code)]

fn f<'FooBar>( // { dg-error "" "" { target *-*-* } }
    _: &'FooBar ()
) {}

fn main() { }

