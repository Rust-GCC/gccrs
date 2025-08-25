// aux-build:stability_attribute_issue.rs
#![deny(deprecated)]

extern crate stability_attribute_issue;
use stability_attribute_issue::*;

fn main() {
    unstable();
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    unstable_msg();
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

