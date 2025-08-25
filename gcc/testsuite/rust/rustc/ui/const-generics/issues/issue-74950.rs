// [full] build-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]


#[derive(PartialEq, Eq)]
struct Inner;

// Note: We emit the error 5 times if we don't deduplicate:
// - struct definition
// - impl PartialEq
// - impl Eq
// - impl StructuralPartialEq
// - impl StructuralEq
#[derive(PartialEq, Eq)]
struct Outer<const I: Inner>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-5 }

fn main() {}

