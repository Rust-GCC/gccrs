// check-pass
// run-rustfix
// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]
#![warn(unused_braces)]


struct A<const N: usize>;

fn main() {
    let _: A<7>; // ok
    let _: A<{ 7 }>; // { dg-warning "" "" { target *-*-* } }
    let _: A<{ 3 + 5 }>; // ok
}

