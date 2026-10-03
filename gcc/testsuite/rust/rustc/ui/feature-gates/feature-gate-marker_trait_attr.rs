use std::fmt::{Debug, Display};

#[marker] trait ExplicitMarker {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }

impl<T: Display> ExplicitMarker for T {}
impl<T: Debug> ExplicitMarker for T {}

fn main() {}

