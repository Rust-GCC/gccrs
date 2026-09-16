#![feature(unboxed_closures)]

trait Zero { fn dummy(&self); }

fn foo(_: dyn Zero())
// { dg-error ".E0220." "" { target *-*-* } .-1 }
// { dg-error ".E0220." "" { target *-*-* } .-2 }
{}

fn main() { }

