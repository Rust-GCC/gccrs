// Test that `assert` works when `const_panic` is enabled.

// revisions: stock const_panic

#![cfg_attr(const_panic, feature(const_panic))]

const _: () = assert!(true);
// { dg-error "" "" { target *-*-* } .-1 }

const _: () = assert!(false);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

