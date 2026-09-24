// edition:2018
// aux-build:cross-crate.rs

extern crate cross_crate;
use cross_crate::*;

#[built_in_attr] // { dg-error "" "" { target *-*-* } }
#[tool_mod::skip] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
fn main() {
    let _: built_in_type; // OK
}

