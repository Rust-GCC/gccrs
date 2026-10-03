// This test ensures that or patterns do not allow missing bindings in any of the arms.

// edition:2018

#![feature(or_patterns)]
#![allow(non_camel_case_types)]

fn main() {}

fn check_handling_of_paths() {
    mod bar {
        pub enum foo {
            alpha,
            beta,
            charlie
        }
    }

    use bar::foo::{alpha, charlie};
    let alpha | beta | charlie = alpha; // { dg-error ".E0408." "" { target *-*-* } }
    match Some(alpha) {
        Some(alpha | beta) => {} // { dg-error ".E0408." "" { target *-*-* } }
    }
}

fn check_misc_nesting() {
    enum E<T> { A(T, T), B(T) }
    use E::*;
    enum Vars3<S, T, U> { V1(S), V2(T), V3(U) }
    use Vars3::*;

    // One level:
    const X: E<u8> = B(0);
    let A(a, _) | _ = X; // { dg-error ".E0408." "" { target *-*-* } }
    let _ | B(a) = X; // { dg-error ".E0408." "" { target *-*-* } }
    let A(..) | B(a) = X; // { dg-error ".E0408." "" { target *-*-* } }
    let A(a, _) | B(_) = X; // { dg-error ".E0408." "" { target *-*-* } }
    let A(_, a) | B(_) = X; // { dg-error ".E0408." "" { target *-*-* } }
    let A(a, b) | B(a) = X; // { dg-error ".E0408." "" { target *-*-* } }

    // Two levels:
    const Y: E<E<u8>> = B(B(0));
    let A(A(..) | B(_), _) | B(a) = Y; // { dg-error ".E0408." "" { target *-*-* } }
    let A(A(..) | B(a), _) | B(A(a, _) | B(a)) = Y;
// { dg-error ".E0408." "" { target *-*-* } .-1 }
    let A(A(a, b) | B(c), d) | B(e) = Y;
// { dg-error ".E0408." "" { target *-*-* } .-1 }
// { dg-error ".E0408." "" { target *-*-* } .-2 }
// { dg-error ".E0408." "" { target *-*-* } .-3 }
// { dg-error ".E0408." "" { target *-*-* } .-4 }
// { dg-error ".E0408." "" { target *-*-* } .-5 }
// { dg-error ".E0408." "" { target *-*-* } .-6 }
// { dg-error ".E0408." "" { target *-*-* } .-7 }
// { dg-error ".E0408." "" { target *-*-* } .-8 }

    // Three levels:
    let (
            V1(
// { dg-error ".E0408." "" { target *-*-* } .-1 }
// { dg-error ".E0408." "" { target *-*-* } .-2 }
                A(
                    Ok(a) | Err(_), // { dg-error ".E0408." "" { target *-*-* } }
                    _
                ) |
                B(Ok(a) | Err(a))
            ) |
            V2(
                A(
                    A(_, a) | // { dg-error ".E0408." "" { target *-*-* } }
                    B(b), // { dg-error ".E0408." "" { target *-*-* } }
                    _
                ) |
                B(_)
// { dg-error ".E0408." "" { target *-*-* } .-1 }
// { dg-error ".E0408." "" { target *-*-* } .-2 }
            ) |
            V3(c),
// { dg-error ".E0408." "" { target *-*-* } .-1 }
        )
        : (Vars3<E<Result<u8, u8>>, E<E<u8>>, u8>,)
        = (V3(0),);
}

