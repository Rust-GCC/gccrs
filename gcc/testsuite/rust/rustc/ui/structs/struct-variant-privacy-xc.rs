// aux-build:struct_variant_privacy.rs
extern crate struct_variant_privacy;

fn f(b: struct_variant_privacy::Bar) { // { dg-error ".E0603." "" { target *-*-* } }
    match b {
        struct_variant_privacy::Bar::Baz { a: _a } => {} // { dg-error ".E0603." "" { target *-*-* } }
    }
}

fn main() {}

