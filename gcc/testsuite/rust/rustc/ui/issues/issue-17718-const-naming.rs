#![warn(unused)]
#![deny(warnings)]

const foo: isize = 3;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

