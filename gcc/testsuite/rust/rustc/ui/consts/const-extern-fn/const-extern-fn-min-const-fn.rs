#![feature(const_extern_fn)]

const extern fn unsize(x: &[u8; 3]) -> &[u8] { x }
const unsafe extern "C" fn closure() -> fn() { || {} }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
const unsafe extern fn use_float() { 1.0 + 1.0; }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
const extern "C" fn ptr_cast(val: *const u8) { val as usize; }
// { dg-error ".E0658." "" { target *-*-* } .-1 }


fn main() {}

