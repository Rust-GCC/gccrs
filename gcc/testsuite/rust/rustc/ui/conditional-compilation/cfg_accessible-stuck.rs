#![feature(cfg_accessible)]

#[cfg_accessible(Z)] // { dg-error "" "" { target *-*-* } }
struct S;

#[cfg_accessible(S)] // { dg-error "" "" { target *-*-* } }
struct Z;

fn main() {}

