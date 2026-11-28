#![feature(non_ascii_idents)]

#[no_mangle]
pub fn řųśť() {}  // { dg-error ".E0754." "" { target *-*-* } }

fn main() {}

