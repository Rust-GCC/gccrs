#![stable(feature = "stable_test_feature", since = "1.0.0")]

#![feature(staged_api)]

#[stable(feature = "foo", since = "1.0.0")]
fn foo_stable_1_0_0() {}

#[stable(feature = "foo", since = "1.29.0")]
// { dg-error ".E0711." "" { target *-*-* } .-1 }
fn foo_stable_1_29_0() {}

#[unstable(feature = "foo", issue = "none")]
// { dg-error ".E0711." "" { target *-*-* } .-1 }
fn foo_unstable() {}

fn main() {}

