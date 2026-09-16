// aux-build:test-macros.rs

extern crate test_macros;
use test_macros::recollect_attr;

#[recollect_attr]
struct FieldStruct where {
    field: MissingType1 // { dg-error ".E0412." "" { target *-*-* } }
}

#[recollect_attr]
struct TupleStruct(MissingType2) where; // { dg-error ".E0412." "" { target *-*-* } }

enum MyEnum where {
    Variant(MissingType3) // { dg-error ".E0412." "" { target *-*-* } }
}

fn main() {}

