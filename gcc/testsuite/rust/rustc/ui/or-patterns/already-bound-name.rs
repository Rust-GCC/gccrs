// This test ensures that the "already bound identifier in a product pattern"
// correctly accounts for or-patterns.

#![feature(or_patterns)]

enum E<T> { A(T, T), B(T) }

use E::*;

fn main() {
    let (a, a) = (0, 1); // Standard duplication without an or-pattern.
// { dg-error ".E0416." "" { target *-*-* } .-1 }

    let (a, A(a, _) | B(a)) = (0, A(1, 2));
// { dg-error ".E0416." "" { target *-*-* } .-1 }
// { dg-error ".E0416." "" { target *-*-* } .-2 }

    let (A(a, _) | B(a), a) = (A(0, 1), 2);
// { dg-error ".E0416." "" { target *-*-* } .-1 }

    let A(a, a) | B(a) = A(0, 1);
// { dg-error ".E0416." "" { target *-*-* } .-1 }

    let B(a) | A(a, a) = A(0, 1);
// { dg-error ".E0416." "" { target *-*-* } .-1 }

    match A(0, 1) {
        B(a) | A(a, a) => {} // Let's ensure `match` has no funny business.
// { dg-error ".E0416." "" { target *-*-* } .-1 }
    }

    let B(A(a, _) | B(a)) | A(a, A(a, _) | B(a)) = B(B(1));
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }

    let B(_) | A(A(a, _) | B(a), A(a, _) | B(a)) = B(B(1));
// { dg-error ".E0416." "" { target *-*-* } .-1 }
// { dg-error ".E0416." "" { target *-*-* } .-2 }
// { dg-error ".E0408." "" { target *-*-* } .-3 }

    let B(A(a, _) | B(a)) | A(A(a, _) | B(a), A(a, _) | B(a)) = B(B(1));
// { dg-error ".E0416." "" { target *-*-* } .-1 }
// { dg-error ".E0416." "" { target *-*-* } .-2 }
}

