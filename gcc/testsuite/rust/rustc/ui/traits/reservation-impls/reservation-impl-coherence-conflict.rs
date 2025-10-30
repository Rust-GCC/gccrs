// check that reservation impls are accounted for in negative reasoning.

#![feature(rustc_attrs)]

trait MyTrait {}
#[rustc_reservation_impl="this impl is reserved"]
impl MyTrait for () {}

trait OtherTrait {}
impl OtherTrait for () {}
impl<T: MyTrait> OtherTrait for T {}
// { dg-error ".E0119." "" { target *-*-* } .-1 }

fn main() {}

