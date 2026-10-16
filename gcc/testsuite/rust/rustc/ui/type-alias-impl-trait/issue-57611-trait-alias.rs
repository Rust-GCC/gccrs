// Regression test for issue #57611
// Ensures that we don't ICE
// FIXME: This should compile, but it currently doesn't

#![feature(trait_alias)]
#![feature(type_alias_impl_trait)]

trait Foo {
    type Bar: Baz<Self, Self>;

    fn bar(&self) -> Self::Bar;
}

struct X;

impl Foo for X {
    type Bar = impl Baz<Self, Self>;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }

    fn bar(&self) -> Self::Bar {
        |x| x
    }
}

trait Baz<A: ?Sized, B: ?Sized> = Fn(&A) -> &B;

fn main() {}

