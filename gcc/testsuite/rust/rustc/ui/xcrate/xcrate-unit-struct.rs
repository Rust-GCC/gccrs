// aux-build:xcrate_unit_struct.rs

// Make sure that when we have cross-crate unit structs we don't accidentally
// make values out of cross-crate structs that aren't unit.

extern crate xcrate_unit_struct;

fn main() {
    let _ = xcrate_unit_struct::StructWithFields;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    let _ = xcrate_unit_struct::Struct;
}

