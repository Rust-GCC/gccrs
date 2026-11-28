#![allow(dead_code)]

type Foo = fn(&u8, &u8) -> &u8; // { dg-error ".E0106." "" { target *-*-* } }

fn bar<F: Fn(&u8, &u8) -> &u8>(f: &F) {} // { dg-error ".E0106." "" { target *-*-* } }

fn main() {}

