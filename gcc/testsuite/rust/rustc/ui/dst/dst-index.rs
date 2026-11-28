// Test that overloaded index expressions with DST result types
// can't be used as rvalues

use std::ops::Index;
use std::fmt::Debug;

#[derive(Copy, Clone)]
struct S;

impl Index<usize> for S {
    type Output = str;

    fn index(&self, _: usize) -> &str {
        "hello"
    }
}

#[derive(Copy, Clone)]
struct T;

impl Index<usize> for T {
    type Output = dyn Debug + 'static;

    fn index<'a>(&'a self, idx: usize) -> &'a (dyn Debug + 'static) {
        static x: usize = 42;
        &x
    }
}

fn main() {
    S[0];
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { dg-error ".E0507." "" { target *-*-* } .-2 }
    T[0];
// { dg-error ".E0507." "" { target *-*-* } .-1 }
// { dg-error ".E0507." "" { target *-*-* } .-2 }
}

