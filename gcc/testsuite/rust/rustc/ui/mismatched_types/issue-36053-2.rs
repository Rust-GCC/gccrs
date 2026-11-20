// Regression test for #36053. ICE was caused due to obligations
// being added to a special, dedicated fulfillment cx during
// a probe.

use std::iter::once;
fn main() {
    once::<&str>("str").fuse().filter(|a: &str| true).count();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
// { dg-error ".E0599." "" { target *-*-* } .-2 }
}

