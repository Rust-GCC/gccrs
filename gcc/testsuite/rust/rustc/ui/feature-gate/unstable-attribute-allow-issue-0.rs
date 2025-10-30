// Check that an issue value can be explicitly set to "0" instead of "none"
#![crate_type = "lib"]
#![feature(staged_api)]
#![stable(feature = "stable_test_feature", since = "1.0.0")]

#[unstable(feature = "unstable_test_feature", issue = "0")]
// { dg-error ".E0545." "" { target *-*-* } .-1 }

#[unstable(feature = "unstable_test_feature", issue = "none")]
fn unstable_issue_none() {}

#[unstable(feature = "unstable_test_feature", issue = "something")]
// { dg-error ".E0545." "" { target *-*-* } .-1 }

