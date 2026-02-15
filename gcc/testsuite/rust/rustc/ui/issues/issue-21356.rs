#![allow(unused_macros)]

macro_rules! test { ($wrong:t_ty ..) => () }
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

