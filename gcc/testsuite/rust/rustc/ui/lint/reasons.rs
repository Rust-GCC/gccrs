// check-pass

#![feature(lint_reasons)]

#![warn(elided_lifetimes_in_paths,
// { dg-note "" "" { target *-*-* } .-1 }
        reason = "explicit anonymous lifetimes aid reasoning about ownership")]
#![warn(
    nonstandard_style,
// { dg-note "" "" { target *-*-* } .-1 }
    reason = r#"people shouldn't have to change their usual style habits
to contribute to our project"#
)]
#![allow(unused, reason = "unused code has never killed anypony")]

use std::fmt;

pub struct CheaterDetectionMechanism {}

impl fmt::Debug for CheaterDetectionMechanism {
    fn fmt(&self, fmt: &mut fmt::Formatter) -> fmt::Result {
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
        fmt.debug_struct("CheaterDetectionMechanism").finish()
    }
}

fn main() {
    let Social_exchange_psychology = CheaterDetectionMechanism {};
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
// { help "" "" { target *-*-* } .-4 }
}

