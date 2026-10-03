// edition:2018

#[bench] // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
fn bench() {}

use bench as _; // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
fn main() {}

