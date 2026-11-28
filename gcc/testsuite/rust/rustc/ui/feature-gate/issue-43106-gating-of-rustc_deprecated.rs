// Testing gating of `#[rustc_deprecated]` in "weird" places.
//
// This file sits on its own because these signal errors, making
// this test incompatible with the "warnings only" nature of
// issue-43106-gating-of-builtin-attrs.rs

#![rustc_deprecated()]
// { dg-error ".E0734." "" { target *-*-* } .-1 }

#[rustc_deprecated()]
// { dg-error ".E0734." "" { target *-*-* } .-1 }
mod rustc_deprecated {
    mod inner { #![rustc_deprecated()] }
// { dg-error ".E0734." "" { target *-*-* } .-1 }

    #[rustc_deprecated()] fn f() { }
// { dg-error ".E0734." "" { target *-*-* } .-1 }

    #[rustc_deprecated()] struct S;
// { dg-error ".E0734." "" { target *-*-* } .-1 }
// { dg-error ".E0734." "" { target *-*-* } .-2 }

    #[rustc_deprecated()] type T = S;
// { dg-error ".E0734." "" { target *-*-* } .-1 }

    #[rustc_deprecated()] impl S { }
// { dg-error ".E0734." "" { target *-*-* } .-1 }
}

fn main() {}

