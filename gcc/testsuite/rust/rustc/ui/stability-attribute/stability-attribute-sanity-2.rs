// More checks that stability attributes are used correctly

#![feature(staged_api)]

#![stable(feature = "stable_test_feature", since = "1.0.0")]

#[stable(feature = "a", feature = "b", since = "1.0.0")] // { dg-error ".E0538." "" { target *-*-* } }
fn f1() { }

#[stable(feature = "a", sinse = "1.0.0")] // { dg-error ".E0541." "" { target *-*-* } }
fn f2() { }

#[unstable(feature = "a", issue = "no")]
// { dg-error ".E0545." "" { target *-*-* } .-1 }
fn f3() { }

fn main() { }

