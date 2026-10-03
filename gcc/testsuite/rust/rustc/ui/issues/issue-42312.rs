use std::ops::Deref;

pub trait Foo {
    fn baz(_: Self::Target) where Self: Deref {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

pub fn f(_: dyn ToString) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() { }

