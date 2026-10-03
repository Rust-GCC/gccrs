#![feature(type_alias_impl_trait)]
#![deny(improper_ctypes)]

type A = impl Fn();

pub fn ret_closure() -> A {
    || {}
}

extern "C" {
    pub fn a(_: A);
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

