// aux-build:structs.rs
extern crate structs;

use structs::{NormalStruct, UnitStruct, TupleStruct, FunctionalRecord};

fn main() {
    let fr = FunctionalRecord {
// { dg-error ".E0639." "" { target *-*-* } .-1 }
        first_field: 1920,
        second_field: 1080,
        ..FunctionalRecord::default()
    };

    let ns = NormalStruct { first_field: 640, second_field: 480 };
// { dg-error ".E0639." "" { target *-*-* } .-1 }

    let NormalStruct { first_field, second_field } = ns;
// { dg-error ".E0638." "" { target *-*-* } .-1 }

    let ts = TupleStruct(640, 480);
// { dg-error ".E0423." "" { target *-*-* } .-1 }

    let ts_explicit = structs::TupleStruct(640, 480);
// { dg-error ".E0603." "" { target *-*-* } .-1 }

    let TupleStruct { 0: first_field, 1: second_field } = ts;
// { dg-error ".E0638." "" { target *-*-* } .-1 }

    let us = UnitStruct;
// { dg-error ".E0423." "" { target *-*-* } .-1 }

    let us_explicit = structs::UnitStruct;
// { dg-error ".E0603." "" { target *-*-* } .-1 }

    let UnitStruct { } = us;
// { dg-error ".E0638." "" { target *-*-* } .-1 }
}

// Everything below this is expected to compile successfully.

// We only test matching here as we cannot create non-exhaustive
// structs from another crate. ie. they'll never pass in run-pass tests.
fn match_structs(ns: NormalStruct, ts: TupleStruct, us: UnitStruct) {
    let NormalStruct { first_field, second_field, .. } = ns;

    let TupleStruct { 0: first, 1: second, .. } = ts;

    let UnitStruct { .. } = us;
}

