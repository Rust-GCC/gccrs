// aux-build:foo.rs

extern crate foo;

type Output = Option<Foo>; // { dg-error ".E0412." "" { target *-*-* } }

fn main() {}

