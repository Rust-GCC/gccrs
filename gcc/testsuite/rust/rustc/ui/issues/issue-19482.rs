// Test that a partially specified trait object with unspecified associated
// type does not type-check.

trait Foo {
    type A;

    fn dummy(&self) { }
}

fn bar(x: &dyn Foo) {}
// { dg-error ".E0191." "" { target *-*-* } .-1 }

pub fn main() {}

