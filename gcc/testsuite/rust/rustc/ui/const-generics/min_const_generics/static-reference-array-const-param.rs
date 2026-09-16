#![feature(min_const_generics)]

fn a<const X: &'static [u32]>() {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    a::<{&[]}>();
}

