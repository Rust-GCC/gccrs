// tests the good error message, not "missing module Foo" or something else unexpected

struct Foo;

fn main() {
    Foo::bar();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

