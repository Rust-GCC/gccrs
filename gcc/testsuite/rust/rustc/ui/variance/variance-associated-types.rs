// Test that the variance computation considers types/regions that
// appear in projections to be invariant.

#![feature(rustc_attrs)]

trait Trait<'a> {
    type Type;

    fn method(&'a self) { }
}

#[rustc_variance]
struct Foo<'a, T : Trait<'a>> { // { dg-error ".E0208." "" { target *-*-* } }
    field: (T, &'a ())
}

#[rustc_variance]
struct Bar<'a, T : Trait<'a>> { // { dg-error ".E0208." "" { target *-*-* } }
    field: <T as Trait<'a>>::Type
}

fn main() { }

