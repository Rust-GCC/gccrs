// Obsolete attributes fall back to unstable custom attributes.

#[ab_isize="stdcall"] extern {}
// { dg-error "" "" { target *-*-* } .-1 }

#[fixed_stack_segment] fn f() {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

