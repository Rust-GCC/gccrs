// run-rustfix

#![deny(no_mangle_generic_items)]

#[no_mangle]
pub fn foo<T>() {} // { dg-error "" "" { target *-*-* } }

#[no_mangle]
pub extern fn bar<T>() {} // { dg-error "" "" { target *-*-* } }

#[no_mangle]
pub fn baz(x: &i32) -> &i32 { x }

#[no_mangle]
pub fn qux<'a>(x: &'a i32) -> &i32 { x }

fn main() {}

