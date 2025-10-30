#![feature(const_raw_ptr_to_usize_cast, const_raw_ptr_deref)]

fn main() {}

// unconst and fine
const Y: usize = unsafe { 42usize as *const i32 as usize + 1 };
// unconst and bad, will thus error in miri
const Y2: usize = unsafe { &1 as *const i32 as usize + 1 }; // { dg-error "" "" { target *-*-* } }
// unconst and fine
const Z: i32 = unsafe { *(&1 as *const i32) };
// unconst and bad, will thus error in miri
const Z2: i32 = unsafe { *(42 as *const i32) }; // { dg-error "" "" { target *-*-* } }
const Z3: i32 = unsafe { *(44 as *const i32) }; // { dg-error "" "" { target *-*-* } }

