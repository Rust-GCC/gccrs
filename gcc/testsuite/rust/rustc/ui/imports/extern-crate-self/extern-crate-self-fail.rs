extern crate self; // { dg-error "" "" { target *-*-* } }

#[macro_use] // { dg-error "" "" { target *-*-* } }
extern crate self as foo;

fn main() {}

