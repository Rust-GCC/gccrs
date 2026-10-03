// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

// Currently, const parameters cannot depend on other generic parameters,
// as our current implementation can't really support this.
//
// We may want to lift this restriction in the future.

pub struct Dependent<const N: usize, const X: [u8; N]>([(); N]);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

pub struct SelfDependent<const N: [u8; N]>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

