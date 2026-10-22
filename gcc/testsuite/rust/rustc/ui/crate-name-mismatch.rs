// compile-flags: --crate-name foo

#![crate_name = "bar"]
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

