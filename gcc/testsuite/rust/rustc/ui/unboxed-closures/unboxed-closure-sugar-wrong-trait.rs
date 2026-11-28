#![feature(unboxed_closures)]

trait Trait {}

fn f<F:Trait(isize) -> isize>(x: F) {}
// { dg-error ".E0220." "" { target *-*-* } .-1 }
// { dg-error ".E0220." "" { target *-*-* } .-2 }

fn main() {}

