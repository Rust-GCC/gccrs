// Prefix in imports with empty braces should be resolved and checked privacy, stability, etc.

mod m {
    mod n {}
}

use m::n::{};
// { dg-error ".E0603." "" { target *-*-* } .-1 }

fn main() {}

