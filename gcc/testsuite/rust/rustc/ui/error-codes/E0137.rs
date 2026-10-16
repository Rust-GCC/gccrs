#![feature(main)]

#[main]
fn foo() {}

#[main]
fn f() {}
// { dg-error ".E0137." "" { target *-*-* } .-1 }

