#![feature(plugin)]
#![plugin(foo="bleh")] // { dg-error ".E0498." "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }

fn main() {}

