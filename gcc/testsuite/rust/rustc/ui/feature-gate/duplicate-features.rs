#![allow(stable_features)]

#![feature(rust1)]
#![feature(rust1)] // { dg-error ".E0636." "" { target *-*-* } }

#![feature(if_let)]
#![feature(if_let)] // { dg-error ".E0636." "" { target *-*-* } }

fn main() {}

