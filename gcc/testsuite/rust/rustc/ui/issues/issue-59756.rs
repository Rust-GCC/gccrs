// run-rustfix
// ignore-test
//
// FIXME: Re-enable this test once we support choosing
// between multiple mutually exclusive suggestions for the same span

#![allow(warnings)]

struct A;
struct B;

fn foo() -> Result<A, B> {
    Ok(A)
}

fn bar() -> Result<A, B> {
    foo()?
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

