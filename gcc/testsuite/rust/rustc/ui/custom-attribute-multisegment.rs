// Unresolved multi-segment attributes are not treated as custom.

mod existent {}

#[existent::nonexistent] // { dg-error ".E0433." "" { target *-*-* } }
fn main() {}

