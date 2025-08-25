#![allow(unused_macros)]

// Issue #21370

macro_rules! test {
    ($wrong:t_ty) => () // { dg-error "" "" { target *-*-* } }
}

fn main() { }

