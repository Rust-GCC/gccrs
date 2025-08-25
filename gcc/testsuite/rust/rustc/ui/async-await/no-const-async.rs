// edition:2018
// compile-flags: --crate-type lib

pub const async fn x() {}
// { dg-error "" "" { target *-*-* } .-1 }

