#![feature(const_generics, const_evaluatable_checked)]
#![allow(incomplete_features)]

fn add<const N: usize>() -> [u8; { N + 1; 5 }] {
// { dg-error "" "" { target *-*-* } .-1 }
    todo!()
}

fn div<const N: usize>() -> [u8; { N / 1; 5 }] {
// { dg-error "" "" { target *-*-* } .-1 }
    todo!()
}

const fn foo(n: usize) {}

fn fn_call<const N: usize>() -> [u8; { foo(N); 5 }] {
// { dg-error "" "" { target *-*-* } .-1 }
    todo!()
}

fn main() {
    add::<12>();
    div::<9>();
    fn_call::<14>();
}

