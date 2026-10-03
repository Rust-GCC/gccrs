// Check that the `'_` used in structs/enums gives an error.

use std::fmt::Debug;

struct Foo {
    x: &'_ u32, // { dg-error ".E0106." "" { target *-*-* } }
}

enum Bar {
    Variant(&'_ u32), // { dg-error ".E0106." "" { target *-*-* } }
}

fn main() { }

