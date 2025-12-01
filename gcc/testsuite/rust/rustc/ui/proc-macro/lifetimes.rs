// aux-build:lifetimes.rs

extern crate lifetimes;

use lifetimes::*;

type A = single_quote_alone!(); // { dg-error "" "" { target *-*-* } }

fn main() {}

