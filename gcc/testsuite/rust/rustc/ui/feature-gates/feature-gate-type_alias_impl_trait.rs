// ignore-compare-mode-chalk
use std::fmt::Debug;

type Foo = impl Debug; // { dg-error ".E0658." "" { target *-*-* } }

trait Bar {
    type Baa: Debug;
    fn define() -> Self::Baa;
}

impl Bar for () {
    type Baa = impl Debug; // { dg-error ".E0658." "" { target *-*-* } }
    fn define() -> Self::Baa {
        0
    }
}

fn define() -> Foo {
    0
}

trait TraitWithDefault {
    type Assoc = impl Debug;
// { dg-error ".E0562." "" { target *-*-* } .-1 }
// { dg-error ".E0562." "" { target *-*-* } .-2 }
// { dg-error ".E0562." "" { target *-*-* } .-3 }
}

type NestedFree = (Vec<impl Debug>, impl Debug, impl Iterator<Item = impl Debug>);
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
// { dg-error ".E0658." "" { target *-*-* } .-4 }

fn define_multiple() -> NestedFree {
    (vec![true], 0u8, 0i32..1)
}

impl Bar for u8 {
    type Baa = (Vec<impl Debug>, impl Debug, impl Iterator<Item = impl Debug> + Debug);
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
// { dg-error ".E0658." "" { target *-*-* } .-4 }
    fn define() -> Self::Baa {
        (vec![true], 0u8, 0i32..1)
    }
}

fn main() {}

