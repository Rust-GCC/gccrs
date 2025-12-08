#![feature(generators)]

fn main() { yield; }
// { dg-error ".E0627." "" { target *-*-* } .-1 }

