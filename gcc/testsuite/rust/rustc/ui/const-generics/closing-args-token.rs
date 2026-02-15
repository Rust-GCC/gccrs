// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct S<const X: u32>;
struct T<const X: bool>;

fn bad_args_1() {
    S::<5 + 2 >> 7>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn bad_args_2() {
    S::<{ 5 + 2 } >> 7>;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn bad_args_3() {
    T::<0 >= 3>;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn bad_args_4() {
    let mut x = 0;
    T::<x >>= 2 > 0>;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

