// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

trait Foo {}

impl<const N: usize> Foo for [(); N]
    where
        Self:FooImpl<{N==0}>
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
{}

trait FooImpl<const IS_ZERO: bool>{}

impl FooImpl<true> for [(); 0] {}

impl<const N:usize> FooImpl<false> for [();N] {}

fn foo(_: impl Foo) {}

fn main() {
    foo([]);
    foo([()]);
}

