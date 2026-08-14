// build-fail

// Regression test for #66975
#![warn(const_err)]
#![feature(const_panic)]
#![feature(never_type)]

const VOID: ! = panic!();
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {
    let _ = VOID;
// { dg-error ".E0080." "" { target *-*-* } .-1 }
}

