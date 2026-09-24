// { dg-note "" "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
// This is testing whether various builtin attributes signals an
// error or warning when put in "weird" places.
//
// (This file sits on its own because it actually signals an error,
// which would mess up the treatment of other cases in
// issue-43106-gating-of-builtin-attrs.rs)

// ignore-tidy-linelength

#![macro_export]
// { dg-error "" "" { target *-*-* } .-1 }
#![main]
// { dg-error "" "" { target *-*-* } .-1 }
#![start]
// { dg-error "" "" { target *-*-* } .-1 }
#![repr()]
// { dg-error "" "" { target *-*-* } .-1 }
#![path = "3800"]
// { dg-error "" "" { target *-*-* } .-1 }
#![automatically_derived]
// { dg-error "" "" { target *-*-* } .-1 }
#![no_mangle]
#![no_link]
// { dg-error "" "" { target *-*-* } .-1 }
#![export_name = "2200"]
// { dg-error "" "" { target *-*-* } .-1 }
#![inline]
// { dg-error ".E0518." "" { target *-*-* } .-1 }
#[inline]
// { dg-error ".E0518." "" { target *-*-* } .-1 }
mod inline {
// { dg-note "" "" { target *-*-* } .-1 }

    mod inner { #![inline] }
// { dg-error ".E0518." "" { target *-*-* } .-1 }
// { dg-note ".E0518." "" { target *-*-* } .-2 }

    #[inline = "2100"] fn f() { }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
// { dg-note "" "" { target *-*-* } .-4 }

    #[inline] struct S;
// { dg-error ".E0518." "" { target *-*-* } .-1 }
// { dg-note ".E0518." "" { target *-*-* } .-2 }

    #[inline] type T = S;
// { dg-error ".E0518." "" { target *-*-* } .-1 }
// { dg-note ".E0518." "" { target *-*-* } .-2 }

    #[inline] impl S { }
// { dg-error ".E0518." "" { target *-*-* } .-1 }
// { dg-note ".E0518." "" { target *-*-* } .-2 }
}

#[no_link]
// { dg-error "" "" { target *-*-* } .-1 }
mod no_link {
// { dg-note "" "" { target *-*-* } .-1 }

    mod inner { #![no_link] }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

    #[no_link] fn f() { }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

    #[no_link] struct S;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

    #[no_link]type T = S;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

    #[no_link] impl S { }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
}

#[export_name = "2200"]
// { dg-error "" "" { target *-*-* } .-1 }
mod export_name {
// { dg-note "" "" { target *-*-* } .-1 }

    mod inner { #![export_name="2200"] }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

    #[export_name = "2200"] fn f() { }

    #[export_name = "2200"] struct S;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

    #[export_name = "2200"] type T = S;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

    #[export_name = "2200"] impl S { }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
}

#[main]
// { dg-error "" "" { target *-*-* } .-1 }
mod main {
    mod inner { #![main] }
// { dg-error "" "" { target *-*-* } .-1 }

    // for `fn f()` case, see feature-gate-main.rs

    #[main] struct S;
// { dg-error "" "" { target *-*-* } .-1 }

    #[main] type T = S;
// { dg-error "" "" { target *-*-* } .-1 }

    #[main] impl S { }
// { dg-error "" "" { target *-*-* } .-1 }
}

#[start]
// { dg-error "" "" { target *-*-* } .-1 }
mod start {
    mod inner { #![start] }
// { dg-error "" "" { target *-*-* } .-1 }

    // for `fn f()` case, see feature-gate-start.rs

    #[start] struct S;
// { dg-error "" "" { target *-*-* } .-1 }

    #[start] type T = S;
// { dg-error "" "" { target *-*-* } .-1 }

    #[start] impl S { }
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

