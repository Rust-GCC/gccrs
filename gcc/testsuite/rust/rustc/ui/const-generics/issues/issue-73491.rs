// [full] check-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

const LEN: usize = 1024;

fn hoge<const IN: [u32; LEN]>() {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

