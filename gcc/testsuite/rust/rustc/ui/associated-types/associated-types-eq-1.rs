// Test equality constraints on associated types. Check that unsupported syntax
// does not ICE.

pub trait Foo {
    type A;
    fn boo(&self) -> <Self as Foo>::A;
}

fn foo2<I: Foo>(x: I) {
    let _: A = x.boo(); // { dg-error ".E0412." "" { target *-*-* } }
}

pub fn main() {}

