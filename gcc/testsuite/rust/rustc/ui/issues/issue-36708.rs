// aux-build:issue-36708.rs

extern crate issue_36708 as lib;

struct Bar;

impl lib::Foo for Bar {
    fn foo<T>() {}
// { dg-error ".E0049." "" { target *-*-* } .-1 }
}

fn main() {}

