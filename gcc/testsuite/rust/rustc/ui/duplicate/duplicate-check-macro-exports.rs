pub use std::panic;

#[macro_export]
macro_rules! panic { () => {} } // { dg-error ".E0255." "" { target *-*-* } }

fn main() {}

