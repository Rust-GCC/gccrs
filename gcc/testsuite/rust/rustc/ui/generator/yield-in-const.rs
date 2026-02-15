#![feature(generators)]

const A: u8 = { yield 3u8; 3u8};
// { dg-error ".E0627." "" { target *-*-* } .-1 }

fn main() {}

