#![feature(rustc_private)]

extern crate libc;

fn main() {
    let ptr: *mut () = core::ptr::null_mut();
    let _: &mut dyn Fn() = unsafe {
        &mut *(ptr as *mut dyn Fn())
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    };
}

