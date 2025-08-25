// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct Bad<const N: usize, T> {
// { dg-error "" "" { target *-*-* } .-1 }
    arr: [u8; { N }],
    another: T,
}

struct AlsoBad<const N: usize, 'a, T, 'b, const M: usize, U> {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    a: &'a T,
    b: &'b U,
}

fn main() {
    let _: AlsoBad<7, 'static, u32, 'static, 17, u16>;
// { dg-error "" "" { target *-*-* } .-1 }
 }

