// Test that an `impl Trait` type that expands to itself is an error.

#![allow(unconditional_recursion)]

fn test() -> impl Sized {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    test()
}

fn main() {}

