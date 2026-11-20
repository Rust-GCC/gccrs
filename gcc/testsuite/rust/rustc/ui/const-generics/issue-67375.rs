// revisions: full min

#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(min, feature(min_const_generics))]

struct Bug<T> {
// { dg-error "" "" { target *-*-* } .-1 }
    inner: [(); { [|_: &T| {}; 0].len() }],
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
}

fn main() {}

