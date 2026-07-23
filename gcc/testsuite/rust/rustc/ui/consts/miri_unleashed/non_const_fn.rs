// compile-flags: -Zunleash-the-miri-inside-of-you

#![allow(const_err)]

// A test demonstrating that we prevent calling non-const fn during CTFE.

fn foo() {}

static C: () = foo();
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-note ".E0080." "" { target *-*-* } .-2 }

fn main() {}

