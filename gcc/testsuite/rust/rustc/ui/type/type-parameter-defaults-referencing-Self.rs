// Test a default that references `Self` which is then used in an object type.
// Issue #18956.

#![feature(default_type_params)]

trait Foo<T=Self> {
    fn method(&self);
}

fn foo(x: &dyn Foo) { }
// { dg-error ".E0393." "" { target *-*-* } .-1 }

fn main() { }

