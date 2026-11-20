// Check that specialization traits can't be implemented without a feature.

// gate-test-min_specialization

// aux-build:specialization-trait.rs

extern crate specialization_trait;

struct A {}

impl specialization_trait::SpecTrait for A {
// { dg-error "" "" { target *-*-* } .-1 }
    fn method(&self) {}
}

fn main() {}

