// run-rustfix

#![allow(dead_code)]
#![allow(unused_variables)]
fn f(
                                     x: u8
                                     y: u8,
) {}
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

