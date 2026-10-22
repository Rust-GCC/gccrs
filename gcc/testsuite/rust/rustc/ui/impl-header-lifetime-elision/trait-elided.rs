#![allow(warnings)]

trait MyTrait<'a> { }

impl MyTrait for u32 {
// { dg-error ".E0726." "" { target *-*-* } .-1 }
}

fn main() {}

