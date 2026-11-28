// Test that specializing on two type parameters being equal is not allowed.

#![feature(min_specialization)]

trait X {
    fn f();
}

impl<T> X for T {
    default fn f() {}
}
impl<T> X for (T, T) {
// { dg-error "" "" { target *-*-* } .-1 }
    fn f() {}
}

fn main() {}

