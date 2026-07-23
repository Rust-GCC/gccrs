// check-pass
// aux-build:attributes-on-definitions.rs

#![forbid(unsafe_code)]

extern crate attributes_on_definitions;

attributes_on_definitions::with_attrs!();
// { dg-warning "" "" { target *-*-* } .-1 }
// No errors about the use of unstable and unsafe code inside the macro.

fn main() {}

