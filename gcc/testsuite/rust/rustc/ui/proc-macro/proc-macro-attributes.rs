// aux-build:derive-b.rs

#[macro_use]
extern crate derive_b;

#[B] // { dg-error ".E0659." "" { target *-*-* } }
#[C] // { dg-error "" "" { target *-*-* } }
#[B(D)] // { dg-error ".E0659." "" { target *-*-* } }
#[B(E = "foo")] // { dg-error ".E0659." "" { target *-*-* } }
#[B(arbitrary tokens)] // { dg-error ".E0659." "" { target *-*-* } }
#[derive(B)]
struct B;

fn main() {}

