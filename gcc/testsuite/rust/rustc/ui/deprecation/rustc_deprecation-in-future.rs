// ignore-tidy-linelength

#![deny(deprecated_in_future)]

#![feature(staged_api)]

#![stable(feature = "rustc_deprecation-in-future-test", since = "1.0.0")]

#[rustc_deprecated(since = "99.99.99", reason = "effectively never")]
#[stable(feature = "rustc_deprecation-in-future-test", since = "1.0.0")]
pub struct S;

fn main() {
    let _ = S; // { dg-error "" "" { target *-*-* } }
}

