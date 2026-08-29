// edition:2018

trait Trait2015 { fn foo(#[allow(C)] i32); }
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

