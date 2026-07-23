#![allow(unused_macros)]

macro_rules! test {
    ($e:expr +) => () // { dg-error "" "" { target *-*-* } }
}

fn main() { }

