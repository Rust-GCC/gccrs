#![feature(unboxed_closures)]

trait Three<A,B,C> { fn dummy(&self) -> (A,B,C); }

fn foo(_: &dyn Three())
// { dg-error ".E0220." "" { target *-*-* } .-1 }
// { dg-error ".E0220." "" { target *-*-* } .-2 }
{}

fn main() { }

