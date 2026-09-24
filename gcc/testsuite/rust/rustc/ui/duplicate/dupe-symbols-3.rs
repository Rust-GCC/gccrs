// build-fail

//
#![crate_type="rlib"]
#![allow(warnings)]

#[export_name="fail"]
pub fn a() {
}

#[no_mangle]
pub fn fail() {
// { dg-error "" "" { target *-*-* } .-1 }
}

