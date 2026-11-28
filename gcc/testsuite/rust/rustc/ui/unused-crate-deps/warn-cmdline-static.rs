// Check for unused crate dep, no path

// edition:2018
// check-pass
// compile-flags: -Wunused-crate-dependencies
// aux-crate:bar=bar.rs
// no-prefer-dynamic

fn main() {}
// { dg-warning "" "" { target *-*-* } .-1 }

