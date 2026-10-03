#![crate_name = "NonSnakeCase"]
// { dg-error "" "" { target *-*-* } .-1 }
#![deny(non_snake_case)]

fn main() {}

