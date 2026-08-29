// compile-flags: --test

#![deny(dead_code)]

fn dead() {} // { dg-error "" "" { target *-*-* } }

fn main() {}

