// ignore-tidy-linelength

#![feature(rustc_attrs)]

#![allow(unused)]

#[rustc_on_unimplemented = "test error `{Self}` with `{Bar}` `{Baz}` `{Quux}`"]
trait Foo<Bar, Baz, Quux>
{}

#[rustc_on_unimplemented="a collection of type `{Self}` cannot be built from an iterator over elements of type `{A}`"]
trait MyFromIterator<A> {
    /// Builds a container with elements from an external iterator.
    fn my_from_iter<T: Iterator<Item=A>>(iterator: T) -> Self;
}

#[rustc_on_unimplemented]
// { dg-error "" "" { target *-*-* } .-1 }
trait BadAnnotation1
{}

#[rustc_on_unimplemented = "Unimplemented trait error on `{Self}` with params `<{A},{B},{C}>`"]
// { dg-error ".E0230." "" { target *-*-* } .-1 }
trait BadAnnotation2<A,B>
{}

#[rustc_on_unimplemented = "Unimplemented trait error on `{Self}` with params `<{A},{B},{}>`"]
// { dg-error ".E0231." "" { target *-*-* } .-1 }
trait BadAnnotation3<A,B>
{}

#[rustc_on_unimplemented(lorem="")]
// { dg-error ".E0232." "" { target *-*-* } .-1 }
trait BadAnnotation4 {}

#[rustc_on_unimplemented(lorem(ipsum(dolor)))]
// { dg-error ".E0232." "" { target *-*-* } .-1 }
trait BadAnnotation5 {}

#[rustc_on_unimplemented(message="x", message="y")]
// { dg-error ".E0232." "" { target *-*-* } .-1 }
trait BadAnnotation6 {}

#[rustc_on_unimplemented(message="x", on(desugared, message="y"))]
// { dg-error ".E0232." "" { target *-*-* } .-1 }
trait BadAnnotation7 {}

#[rustc_on_unimplemented(on(), message="y")]
// { dg-error ".E0232." "" { target *-*-* } .-1 }
trait BadAnnotation8 {}

#[rustc_on_unimplemented(on="x", message="y")]
// { dg-error ".E0232." "" { target *-*-* } .-1 }
trait BadAnnotation9 {}

#[rustc_on_unimplemented(on(x="y"), message="y")]
trait BadAnnotation10 {}

#[rustc_on_unimplemented(on(desugared, on(desugared, message="x")), message="y")]
// { dg-error ".E0232." "" { target *-*-* } .-1 }
trait BadAnnotation11 {}

pub fn main() {
}

