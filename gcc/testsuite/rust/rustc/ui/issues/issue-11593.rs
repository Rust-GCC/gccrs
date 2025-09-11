// aux-build:private-trait-xc.rs

extern crate private_trait_xc;

struct Bar;

impl private_trait_xc::Foo for Bar {}
// { dg-error ".E0603." "" { target *-*-* } .-1 }

fn main() {}

