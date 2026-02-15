#![allow(warnings)]

const X: u8 =
    || -> u8 { 5 }()
// { dg-error ".E0015." "" { target *-*-* } .-1 }
;

fn main() {}

