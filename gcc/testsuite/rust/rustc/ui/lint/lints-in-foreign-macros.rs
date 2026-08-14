// aux-build:lints-in-foreign-macros.rs
// check-pass

#![warn(unused_imports)] // { dg-error "" "" { target *-*-* } }
#![warn(missing_docs)]

#[macro_use]
extern crate lints_in_foreign_macros;

macro_rules! foo {
    () => {use std::string::ToString;} // { dg-warning "" "" { target *-*-* } }
}

mod a { foo!(); }
mod b { bar!(); }
mod c { baz!(use std::string::ToString;); } // { dg-warning "" "" { target *-*-* } }
mod d { baz2!(use std::string::ToString;); } // { dg-warning "" "" { target *-*-* } }
baz!(pub fn undocumented() {}); // { dg-warning "" "" { target *-*-* } }
baz2!(pub fn undocumented2() {}); // { dg-warning "" "" { target *-*-* } }

fn main() {}

