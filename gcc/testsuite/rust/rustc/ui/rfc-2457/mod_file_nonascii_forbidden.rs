#![feature(non_ascii_idents)]

mod řųśť; // { dg-error ".E0754." "" { target *-*-* } }
// { dg-error ".E0754." "" { target *-*-* } .-1 }

fn main() {}

