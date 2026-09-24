// Check that different const types are different.
// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct Const<const V: [usize; 1]> {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    let mut x = Const::<{ [3] }> {};
    x = Const::<{ [4] }> {};
// { dg-error "" "" { target *-*-* } .-1 }
}

