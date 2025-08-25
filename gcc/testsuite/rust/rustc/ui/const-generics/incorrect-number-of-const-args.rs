// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

fn foo<const X: usize, const Y: usize>() -> usize {
    0
}

fn main() {
    foo::<0>(); // { dg-error "" "" { target *-*-* } }
    foo::<0, 0, 0>(); // { dg-error "" "" { target *-*-* } }
}

