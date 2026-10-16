// Tests that a default impl still has to have a WF trait ref.

#![feature(specialization)] // { dg-warning "" "" { target *-*-* } }

trait Foo<'a, T: Eq + 'a> { }

default impl<U> Foo<'static, U> for () {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main(){}

