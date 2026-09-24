#![allow(unused_macros)]

macro_rules! invalid {
    _ => (); // { dg-error "" "" { target *-*-* } }
}

fn main() {
}

