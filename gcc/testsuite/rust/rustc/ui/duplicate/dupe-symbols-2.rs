// build-fail

//
#![crate_type="rlib"]
#![allow(warnings)]

pub mod a {
    #[no_mangle]
    pub extern fn fail() {
    }
}

pub mod b {
    #[no_mangle]
    pub extern fn fail() {
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

