#![deny(unused)]

#[macro_use] // { dg-error "" "" { target *-*-* } }
extern crate core;

#[macro_use(
    panic // { dg-error "" "" { target *-*-* } }
)]
extern crate core as core_2;

fn main() {}

