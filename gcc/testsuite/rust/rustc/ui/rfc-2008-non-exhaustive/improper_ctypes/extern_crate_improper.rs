// aux-build:types.rs
#![deny(improper_ctypes)]

extern crate types;

// This test checks that non-exhaustive types with `#[repr(C)]` from an extern crate are considered
// improper.

use types::{NonExhaustiveEnum, NormalStruct, UnitStruct, TupleStruct, NonExhaustiveVariants};

extern {
    pub fn non_exhaustive_enum(_: NonExhaustiveEnum);
// { dg-error "" "" { target *-*-* } .-1 }
    pub fn non_exhaustive_normal_struct(_: NormalStruct);
// { dg-error "" "" { target *-*-* } .-1 }
    pub fn non_exhaustive_unit_struct(_: UnitStruct);
// { dg-error "" "" { target *-*-* } .-1 }
    pub fn non_exhaustive_tuple_struct(_: TupleStruct);
// { dg-error "" "" { target *-*-* } .-1 }
    pub fn non_exhaustive_variant(_: NonExhaustiveVariants);
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() { }

