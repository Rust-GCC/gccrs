// Test some cases where or-patterns may ostensibly be allowed but are in fact not.
// This is not a semantic test. We only test parsing.

#![feature(or_patterns)]

fn main() {}

// Test the `pat` macro fragment parser:
macro_rules! accept_pat {
    ($p:pat) => {}
}

accept_pat!(p | q); // { dg-error "" "" { target *-*-* } }
accept_pat!(| p | q); // { dg-error "" "" { target *-*-* } }

// Non-macro tests:

enum E { A, B }
use E::*;

fn no_top_level_or_patterns() {
    // We do *not* allow or-patterns at the top level of lambdas...
    let _ = |A | B: E| (); // { dg-error ".E0369." "" { target *-*-* } }
    //           -------- This looks like an or-pattern but is in fact `|A| (B: E | ())`.

    // ...and for now neither do we allow or-patterns at the top level of functions.
    fn fun1(A | B: E) {} // { dg-error "" "" { target *-*-* } }

    fn fun2(| A | B: E) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

// We also do not allow a leading `|` when not in a top level position:

fn no_leading_inner() {
    struct TS(E);
    struct NS { f: E }

    let ( | A | B) = E::A; // { dg-error "" "" { target *-*-* } }
    let ( | A | B,) = (E::B,); // { dg-error "" "" { target *-*-* } }
    let [ | A | B ] = [E::A]; // { dg-error "" "" { target *-*-* } }
    let TS( | A | B ); // { dg-error "" "" { target *-*-* } }
    let NS { f: | A | B }; // { dg-error "" "" { target *-*-* } }

    let ( || A | B) = E::A; // { dg-error "" "" { target *-*-* } }
    let [ || A | B ] = [E::A]; // { dg-error "" "" { target *-*-* } }
    let TS( || A | B ); // { dg-error "" "" { target *-*-* } }
    let NS { f: || A | B }; // { dg-error "" "" { target *-*-* } }

    let recovery_witness: String = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

