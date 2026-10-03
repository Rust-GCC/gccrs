// build-fail

//
#![crate_type="rlib"]
#![allow(warnings)]

#[export_name="fail"]
pub fn a() {
}

#[export_name="fail"]
pub fn b() {
// { dg-error "" "" { target *-*-* } .-1 }
}

