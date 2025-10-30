// aux-build:uninhabited.rs
#![deny(unreachable_patterns)]
#![feature(exhaustive_patterns)]
#![feature(never_type)]

extern crate uninhabited;

use uninhabited::{
    IndirectUninhabitedEnum,
    IndirectUninhabitedStruct,
    IndirectUninhabitedTupleStruct,
    IndirectUninhabitedVariants,
};

struct A;

// This test checks that an empty match on a non-exhaustive uninhabited type through a level of
// indirection from an extern crate will not compile. In particular, this enables the
// `exhaustive_patterns` feature as this can change the branch used in the compiler to determine
// this.

fn cannot_empty_match_on_empty_enum_to_anything(x: IndirectUninhabitedEnum) -> A {
    match x {} // { dg-error ".E0004." "" { target *-*-* } }
}

fn cannot_empty_match_on_empty_struct_to_anything(x: IndirectUninhabitedStruct) -> A {
    match x {} // { dg-error ".E0004." "" { target *-*-* } }
}

fn cannot_empty_match_on_empty_tuple_struct_to_anything(x: IndirectUninhabitedTupleStruct) -> A {
    match x {} // { dg-error ".E0004." "" { target *-*-* } }
}

fn cannot_empty_match_on_enum_with_empty_variants_struct_to_anything(
    x: IndirectUninhabitedVariants,
) -> A {
    match x {} // { dg-error ".E0004." "" { target *-*-* } }
}

fn main() {}

