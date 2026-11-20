#![allow(foo = "")] // { dg-error ".E0452." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-5 }
// { dg-error "" "" { target *-*-* } .-6 }
fn main() {
}

