// Check that we error when `'_` appears as the name of a lifetime parameter.
//
// Regression test for #52098.

struct IceCube<'a> {
    v: Vec<&'a char>
}

impl<'_> IceCube<'_> {}
// { dg-error ".E0637." "" { target *-*-* } .-1 }

struct Struct<'_> {
// { dg-error ".E0637." "" { target *-*-* } .-1 }
    v: Vec<&'static char>
}

enum Enum<'_> {
// { dg-error ".E0637." "" { target *-*-* } .-1 }
    Variant
}

union Union<'_> {
// { dg-error ".E0637." "" { target *-*-* } .-1 }
    a: u32
}

trait Trait<'_> {
// { dg-error ".E0637." "" { target *-*-* } .-1 }
}

fn foo<'_>() {
// { dg-error ".E0637." "" { target *-*-* } .-1 }
}

fn main() {}

