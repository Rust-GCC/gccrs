#![feature(unboxed_closures)]

fn foo<F: Fn<i32>>(f: F) -> F::Output { f(3) } // { dg-error ".E0059." "" { target *-*-* } }

fn main() {
}

