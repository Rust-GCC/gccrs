// Test that we do not ICE when an impl is missing an associated type (and that we report
// a useful error, of course).

trait Trait {
    type Type;
}

impl Trait for isize {}  // { dg-error ".E0046." "" { target *-*-* } }

fn main() {}

