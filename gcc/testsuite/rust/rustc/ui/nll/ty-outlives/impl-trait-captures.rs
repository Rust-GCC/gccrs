// compile-flags:-Zborrowck=mir -Zverbose

#![allow(warnings)]

trait Foo<'a> {
}

impl<'a, T> Foo<'a> for T { }

fn foo<'a, T>(x: &T) -> impl Foo<'a> {
// { dg-error ".E0621." "" { target *-*-* } .-1 }
    x
}

fn main() {}

