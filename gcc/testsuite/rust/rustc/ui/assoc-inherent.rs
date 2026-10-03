// Test associated types are, until #8995 is implemented, forbidden in inherent impls.

struct Foo;

impl Foo {
    type Bar = isize; // { dg-error ".E0202." "" { target *-*-* } }
}

fn main() {}

