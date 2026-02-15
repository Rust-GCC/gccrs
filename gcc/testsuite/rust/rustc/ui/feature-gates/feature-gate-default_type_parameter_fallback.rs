#![allow(unused)]

fn avg<T=i32>(_: T) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

struct S<T>(T);
impl<T=i32> S<T> {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

fn main() {}

