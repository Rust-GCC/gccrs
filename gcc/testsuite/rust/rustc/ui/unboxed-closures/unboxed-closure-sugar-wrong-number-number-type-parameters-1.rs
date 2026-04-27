#![feature(unboxed_closures)]

trait One<A> { fn foo(&self) -> A; }

fn foo(_: &dyn One()) // { dg-error ".E0220." "" { target *-*-* } }
{}

fn main() { }

