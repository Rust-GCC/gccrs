trait TraitB {
    type Item;
}

trait TraitA<A> {
    fn foo<T: TraitB<Item = A>>(_: T) -> Self;
    fn bar<T>(_: T) -> Self;
    fn baz<T>(_: T) -> Self where T: TraitB, <T as TraitB>::Item: Copy;
    fn bat<T: TraitB<Item: Copy>>(_: T) -> Self; // { dg-error ".E0658." "" { target *-*-* } }
}

struct S;

impl TraitA<()> for S { // { dg-error ".E0046." "" { target *-*-* } }
}

use std::iter::FromIterator;
struct X;
impl FromIterator<()> for X { // { dg-error ".E0046." "" { target *-*-* } }
}

fn main() {}

