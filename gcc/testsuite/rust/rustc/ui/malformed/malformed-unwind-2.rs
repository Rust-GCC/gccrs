#![feature(unwind_attributes)]

#[unwind(allowed, aborts)]
// { dg-error ".E0633." "" { target *-*-* } .-1 }
extern "C" fn f1() {}

#[unwind(unsupported)]
// { dg-error ".E0633." "" { target *-*-* } .-1 }
extern "C" fn f2() {}

fn main() {}

