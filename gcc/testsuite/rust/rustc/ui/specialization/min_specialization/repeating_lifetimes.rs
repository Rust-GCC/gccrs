// Test that directly specializing on repeated lifetime parameters is not
// allowed.

#![feature(min_specialization)]

trait X {
    fn f();
}

impl<T> X for T {
    default fn f() {}
}

impl<'a> X for (&'a u8, &'a u8) {
// { dg-error "" "" { target *-*-* } .-1 }
    fn f() {}
}

fn main() {}

