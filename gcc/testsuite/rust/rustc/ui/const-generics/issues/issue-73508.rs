// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

pub const fn func_name<const X: *const u32>() {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

