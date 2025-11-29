#![allow(incomplete_features)]
#![feature(generic_associated_types)]

use std::ops::Deref;

trait Iterable {
    type Item<'a>;
    type Iter<'a>: Iterator<Item = Self::Item<'a>>
        + Deref<Target = Self::Item<'b>>;
// { dg-error ".E0261." "" { target *-*-* } .-1 }

    fn iter<'a>(&'a self) -> Self::Iter<'undeclared>;
// { dg-error ".E0261." "" { target *-*-* } .-1 }
}

fn main() {}

