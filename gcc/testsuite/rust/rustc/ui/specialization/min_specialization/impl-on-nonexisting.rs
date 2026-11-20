#![feature(min_specialization)]

trait Trait {}
impl Trait for NonExistent {}
// { dg-error ".E0412." "" { target *-*-* } .-1 }

fn main() {}

