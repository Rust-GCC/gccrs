// ignore-cloudabi no std::path

use std::path::Path;

trait Foo {
    fn bar(&self);
}

fn some_func<T: Foo>(foo: T) {
    foo.bar();
}

fn f(p: Path) { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {
    some_func(5i32);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

