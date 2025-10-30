// aux-build:uninhabited.rs
#![feature(never_type)]

extern crate uninhabited;

use uninhabited::{
    UninhabitedEnum,
    UninhabitedStruct,
    UninhabitedTupleStruct,
    UninhabitedVariants,
};

struct A;

// This test checks that an empty match on a non-exhaustive uninhabited type from an extern crate
// will not compile.

fn cannot_empty_match_on_empty_enum_to_anything(x: UninhabitedEnum) -> A {
    match x {} // { dg-error ".E0004." "" { target *-*-* } }
}

fn cannot_empty_match_on_empty_struct_to_anything(x: UninhabitedStruct) -> A {
    match x {} // { dg-error ".E0004." "" { target *-*-* } }
}

fn cannot_empty_match_on_empty_tuple_struct_to_anything(x: UninhabitedTupleStruct) -> A {
    match x {} // { dg-error ".E0004." "" { target *-*-* } }
}

fn cannot_empty_match_on_enum_with_empty_variants_struct_to_anything(x: UninhabitedVariants) -> A {
    match x {} // { dg-error ".E0004." "" { target *-*-* } }
}

fn main() {}

