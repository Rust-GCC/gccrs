// compile-flags: --cap-lints deny

#![warn(unused)]
#![deny(warnings)]

use std::option; // { dg-error "" "" { target *-*-* } }

fn main() {}

