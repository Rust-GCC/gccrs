// build-fail

// Regression test for #66975
#![warn(const_err)]
#![feature(const_panic)]
#![feature(never_type)]

struct PrintName;

impl PrintName {
    const VOID: ! = panic!();
// { dg-warning "" "" { target *-*-* } .-1 }
}

fn main() {
    let _ = PrintName::VOID;
// { dg-error ".E0080." "" { target *-*-* } .-1 }
}

