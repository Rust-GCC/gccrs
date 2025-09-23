#![deny(unused_attributes)]
#![feature(plugin)]

#[plugin(bla)]  // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

fn main() {}

