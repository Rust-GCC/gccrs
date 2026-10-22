#![allow(unused)]

macro_rules! m { ($i) => {} }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

fn main() {}

