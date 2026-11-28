// Test that disallow lifetime parameters that are unused.

use std::marker;

struct Bivariant<'a>; // { dg-error ".E0392." "" { target *-*-* } }

struct Struct<'a, 'd> { // { dg-error ".E0392." "" { target *-*-* } }
    field: &'a [i32]
}

trait Trait<'a, 'd> { // OK on traits
    fn method(&'a self);
}

fn main() {}

