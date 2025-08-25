// ignore-tidy-linelength

// Test that `#[rustc_*]` attributes are gated by `rustc_attrs` feature gate.

#[rustc_variance] // { dg-error ".E0658." "" { target *-*-* } }
#[rustc_error] // { dg-error ".E0658." "" { target *-*-* } }
#[rustc_nonnull_optimization_guaranteed] // { dg-error ".E0658." "" { target *-*-* } }

fn main() {}

