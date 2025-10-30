// aux-build:variant-namespacing.rs

enum E {
    Struct { a: u8 },
    Tuple(u8),
    Unit,
}

type Struct = u8;
type Tuple = u8;
type Unit = u8;
type XStruct = u8;
type XTuple = u8;
type XUnit = u8;

const Struct: u8 = 0;
const Tuple: u8 = 0;
const Unit: u8 = 0;
const XStruct: u8 = 0;
const XTuple: u8 = 0;
const XUnit: u8 = 0;

extern crate variant_namespacing;
pub use variant_namespacing::XE::{XStruct, XTuple, XUnit};
// { dg-error ".E0255." "" { target *-*-* } .-1 }
// { dg-error ".E0255." "" { target *-*-* } .-2 }
// { dg-error ".E0255." "" { target *-*-* } .-3 }
pub use E::{Struct, Tuple, Unit};
// { dg-error ".E0255." "" { target *-*-* } .-1 }
// { dg-error ".E0255." "" { target *-*-* } .-2 }
// { dg-error ".E0255." "" { target *-*-* } .-3 }

fn main() {}

