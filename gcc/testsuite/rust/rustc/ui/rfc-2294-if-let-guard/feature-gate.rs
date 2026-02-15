// gate-test-if_let_guard

use std::ops::Range;

fn _if_let_guard() {
    match () {
        () if let 0 = 1 => {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

        () if (let 0 = 1) => {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

        () if (((let 0 = 1))) => {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

        () if true && let 0 = 1 => {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

        () if let 0 = 1 && true => {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

        () if (let 0 = 1) && true => {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

        () if true && (let 0 = 1) => {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }

        () if (let 0 = 1) && (let 0 = 1) => {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
// { dg-error ".E0658." "" { target *-*-* } .-4 }

        () if let 0 = 1 && let 1 = 2 && (let 2 = 3 && let 3 = 4 && let 4 = 5) => {}
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

        () if let Range { start: _, end: _ } = (true..true) && false => {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
        _ => {}
    }
}

fn _macros() {
    macro_rules! use_expr {
        ($e:expr) => {
            match () {
                () if $e => {}
                _ => {}
            }
        }
    }
    use_expr!((let 0 = 1 && 0 == 0));
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
    use_expr!((let 0 = 1));
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
    match () {
        #[cfg(FALSE)]
        () if let 0 = 1 => {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
        _ => {}
    }
    use_expr!(let 0 = 1);
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

