// aux-build:unreachable_variant.rs

extern crate unreachable_variant as other;

fn main() {
    let _x = other::super_sekrit::sooper_sekrit::baz; // { dg-error ".E0603." "" { target *-*-* } }
}

