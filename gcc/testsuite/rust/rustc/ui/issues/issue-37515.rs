// check-pass

#![warn(unused)]

type Z = dyn for<'x> Send;
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {}

