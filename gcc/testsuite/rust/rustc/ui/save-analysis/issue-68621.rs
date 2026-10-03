// compile-flags: -Zsave-analysis

#![feature(type_alias_impl_trait)]

trait Trait {}

trait Service {
    type Future: Trait;
}

struct Struct;

impl Service for Struct {
    type Future = impl Trait; // { dg-error "" "" { target *-*-* } }
}

fn main() {}

