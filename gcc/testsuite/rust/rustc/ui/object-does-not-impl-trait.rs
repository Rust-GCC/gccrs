// Test that an object type `Box<Foo>` is not considered to implement the
// trait `Foo`. Issue #5087.

trait Foo {}
fn take_foo<F:Foo>(f: F) {}
fn take_object(f: Box<dyn Foo>) { take_foo(f); }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
fn main() {}

