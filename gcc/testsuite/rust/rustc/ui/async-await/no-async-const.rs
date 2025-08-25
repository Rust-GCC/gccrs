// edition:2018
// compile-flags: --crate-type lib

pub async const fn x() {}
// { dg-error "" "" { target *-*-* } .-1 }

