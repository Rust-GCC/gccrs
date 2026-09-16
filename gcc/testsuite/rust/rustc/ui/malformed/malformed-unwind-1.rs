#![feature(unwind_attributes)]

#[unwind] // { dg-error "" "" { target *-*-* } }
extern "C" fn f1() {}

#[unwind = ""] // { dg-error "" "" { target *-*-* } }
extern "C" fn f2() {}

fn main() {}

