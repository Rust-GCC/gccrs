// Test that directly specializing on `'static` is not allowed.

#![feature(min_specialization)]

trait X {
    fn f();
}

impl<T> X for &'_ T {
    default fn f() {}
}

impl X for &'static u8 {
// { dg-error "" "" { target *-*-* } .-1 }
    fn f() {}
}

fn main() {}

