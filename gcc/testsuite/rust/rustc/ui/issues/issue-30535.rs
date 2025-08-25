// aux-build:issue-30535.rs

extern crate issue_30535 as foo;

fn bar(
    _: foo::Foo::FooV // { dg-error ".E0573." "" { target *-*-* } }
) {}

fn main() {}

