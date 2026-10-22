#![feature(plugin)]
#![plugin="bleh"] // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }

fn main() {}

