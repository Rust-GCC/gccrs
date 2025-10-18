// compile-flags: --edition=2018
// run-pass

macro_rules! regex {
// { dg-warning "" "" { target *-*-* } .-1 }
    () => {};
}

#[allow(dead_code)]
use regex;
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {}

