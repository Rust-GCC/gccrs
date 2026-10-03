#![feature(lang_items)]

fn main() {}

#![lang = "foo"] // { dg-error ".E0522." "" { target *-*-* } }
// { dg-error ".E0522." "" { target *-*-* } .-2 }
fn foo() {}

