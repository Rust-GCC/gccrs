// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

use std::ffi::{CStr, CString};

unsafe fn unsafely_do_the_thing<const F: fn(&CStr) -> usize>(ptr: *const i8) -> usize {
// { dg-error "" "" { target *-*-* } .-1 }
    F(CStr::from_ptr(ptr))
}

fn safely_do_the_thing(s: &CStr) -> usize {
    s.to_bytes().len()
}

fn main() {
    let baguette = CString::new("baguette").unwrap();
    let ptr = baguette.as_ptr();
    println!("{}", unsafe {
        unsafely_do_the_thing::<safely_do_the_thing>(ptr)
    });
}

