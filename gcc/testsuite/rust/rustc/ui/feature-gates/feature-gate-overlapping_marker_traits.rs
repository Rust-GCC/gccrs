use std::fmt::{Debug, Display};

trait MyMarker {}

impl<T: Display> MyMarker for T {}
impl<T: Debug> MyMarker for T {}
// { dg-error ".E0119." "" { target *-*-* } .-1 }

fn main() {}

