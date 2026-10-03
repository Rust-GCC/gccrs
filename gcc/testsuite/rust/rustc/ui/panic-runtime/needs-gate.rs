// gate-test-needs_panic_runtime
// gate-test-panic_runtime

#![panic_runtime] // { dg-error ".E0658." "" { target *-*-* } }
#![needs_panic_runtime] // { dg-error ".E0658." "" { target *-*-* } }

fn main() {}

