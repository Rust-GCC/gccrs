// Prefix in imports with empty braces should be resolved and checked privacy, stability, etc.

// aux-build:lint-stability.rs

extern crate lint_stability;

use lint_stability::UnstableEnum::{};
// { dg-error ".E0658." "" { target *-*-* } .-1 }
use lint_stability::StableEnum::{}; // OK

fn main() {}

