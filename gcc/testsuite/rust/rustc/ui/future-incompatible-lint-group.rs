#![deny(future_incompatible)]

trait Tr {
    fn f(u8) {} // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
}

fn main() {}

