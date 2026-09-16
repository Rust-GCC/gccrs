// Tests miscellaneous well-formedness examples.
// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

pub fn arr_len<const N: usize>() {
    let _: [u8; N + 1];
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

struct Const<const N: usize>;

pub fn func_call<const N: usize>() {
    let _: Const::<{N + 1}>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}

