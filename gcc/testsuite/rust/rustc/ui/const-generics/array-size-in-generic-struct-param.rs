// Tests that array sizes that depend on const-params are checked using `ConstEvaluatable`.
// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

#[allow(dead_code)]
struct ArithArrayLen<const N: usize>([u32; 0 + N]);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

#[derive(PartialEq, Eq)]
struct Config {
    arr_size: usize,
}

struct B<const CFG: Config> {
// { dg-error "" "" { target *-*-* } .-1 }
    arr: [u8; CFG.arr_size],
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

const C: Config = Config { arr_size: 5 };

fn main() {
    let b = B::<C> { arr: [1, 2, 3, 4, 5] };
    assert_eq!(b.arr.len(), 5);
}

