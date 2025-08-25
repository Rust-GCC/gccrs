// check-pass
// compile-flags:--test

#![deny(warnings)]

macro_rules! foo {
    () => (fn foo(){})
}

#[test]
foo!(); // { dg-warning "" "" { target *-*-* } }

fn main(){}

