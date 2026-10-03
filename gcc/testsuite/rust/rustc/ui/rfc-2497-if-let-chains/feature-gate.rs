// gate-test-let_chains

// Here we test feature gating for ´let_chains`.
// See `disallowed-positions.rs` for the grammar
// defining the language for gated allowed positions.

#![allow(irrefutable_let_patterns)]

use std::ops::Range;

fn _if() {
    if let 0 = 1 {} // Stable!

    if (let 0 = 1) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    if (((let 0 = 1))) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    if true && let 0 = 1 {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    if let 0 = 1 && true {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    if (let 0 = 1) && true {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    if true && (let 0 = 1) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    if (let 0 = 1) && (let 0 = 1) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
// { dg-error ".E0658." "" { target *-*-* } .-4 }

    if let 0 = 1 && let 1 = 2 && (let 2 = 3 && let 3 = 4 && let 4 = 5) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
// { dg-error ".E0658." "" { target *-*-* } .-4 }
// { dg-error ".E0658." "" { target *-*-* } .-5 }
// { dg-error ".E0658." "" { target *-*-* } .-6 }
// { dg-error ".E0658." "" { target *-*-* } .-7 }
// { dg-error ".E0658." "" { target *-*-* } .-8 }
// { dg-error ".E0658." "" { target *-*-* } .-9 }
// { dg-error ".E0658." "" { target *-*-* } .-10 }

    if let Range { start: _, end: _ } = (true..true) && false {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
}

fn _while() {
    while let 0 = 1 {} // Stable!

    while (let 0 = 1) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    while (((let 0 = 1))) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    while true && let 0 = 1 {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    while let 0 = 1 && true {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    while (let 0 = 1) && true {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    while true && (let 0 = 1) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

    while (let 0 = 1) && (let 0 = 1) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
// { dg-error ".E0658." "" { target *-*-* } .-4 }

    while let 0 = 1 && let 1 = 2 && (let 2 = 3 && let 3 = 4 && let 4 = 5) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
// { dg-error ".E0658." "" { target *-*-* } .-4 }
// { dg-error ".E0658." "" { target *-*-* } .-5 }
// { dg-error ".E0658." "" { target *-*-* } .-6 }
// { dg-error ".E0658." "" { target *-*-* } .-7 }
// { dg-error ".E0658." "" { target *-*-* } .-8 }
// { dg-error ".E0658." "" { target *-*-* } .-9 }
// { dg-error ".E0658." "" { target *-*-* } .-10 }

    while let Range { start: _, end: _ } = (true..true) && false {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
}

fn _macros() {
    macro_rules! noop_expr { ($e:expr) => {}; }

    noop_expr!((let 0 = 1));
// { dg-error ".E0658." "" { target *-*-* } .-1 }

    macro_rules! use_expr {
        ($e:expr) => {
            if $e {}
            while $e {}
        }
    }
    use_expr!((let 0 = 1 && 0 == 0));
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
    use_expr!((let 0 = 1));
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
    #[cfg(FALSE)] (let 0 = 1);
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    use_expr!(let 0 = 1);
// { dg-error "" "" { target *-*-* } .-1 }
    // ^--- FIXME(53667): Consider whether `Let` can be added to `ident_can_begin_expr`.
}

fn main() {}

