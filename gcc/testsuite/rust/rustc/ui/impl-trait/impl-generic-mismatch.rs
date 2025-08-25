use std::fmt::Debug;

trait Foo {
    fn foo(&self, _: &impl Debug);
}

impl Foo for () {
    fn foo<U: Debug>(&self, _: &U) { }
// { dg-error ".E0643." "" { target *-*-* } .-1 }
}

trait Bar {
    fn bar<U: Debug>(&self, _: &U);
}

impl Bar for () {
    fn bar(&self, _: &impl Debug) { }
// { dg-error ".E0643." "" { target *-*-* } .-1 }
}

// With non-local trait (#49841):

use std::hash::{Hash, Hasher};

struct X;

impl Hash for X {
    fn hash(&self, hasher: &mut impl Hasher) {}
// { dg-error ".E0643." "" { target *-*-* } .-1 }
}

fn main() {}

