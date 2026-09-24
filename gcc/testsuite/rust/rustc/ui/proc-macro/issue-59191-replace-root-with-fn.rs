// edition:2018
// aux-crate:issue_59191=issue-59191.rs
// Test that using a macro to replace the entire crate tree with a non-'mod' item errors out nicely.
// `issue_59191::no_main` replaces whatever's passed in with `fn main() {}`.
#![feature(custom_inner_attributes)]
// { dg-error ".E0601." "" { target *-*-* } .-1 }
#![issue_59191::no_main]
// { dg-error "" "" { target *-*-* } .-1 }

