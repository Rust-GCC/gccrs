//! Ideally, these tests would go in `where-allowed.rs`, but we bail out
//! too early to display them.
use std::fmt::Debug;

// Disallowed
fn in_adt_in_return() -> Vec<impl Debug> { panic!() } // { dg-error ".E0720." "" { target *-*-* } }

fn main() {}

