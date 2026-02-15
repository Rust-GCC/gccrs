// compile-flags:--extern му_сгате
// edition:2018
#![feature(non_ascii_idents)]

use му_сгате::baz; // { dg-error "" "" { target *-*-* } }

fn main() {}

