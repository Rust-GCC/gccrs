use std::ops::Deref;

struct Foo;

impl<Foo> Deref for Foo { } // { dg-error ".E0210." "" { target *-*-* } }
// { dg-error ".E0210." "" { target *-*-* } .-1 }

fn main() {}

