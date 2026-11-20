// Test that `ref mut x @ ref mut y` and varieties of that are not allowed.

#![feature(bindings_after_at)]

fn main() {
    struct U;

    fn u() -> U { U }

    fn f1(ref mut a @ ref mut b: U) {}
// { dg-error ".E0499." "" { target *-*-* } .-1 }
    fn f2(ref mut a @ ref mut b: U) {}
// { dg-error ".E0499." "" { target *-*-* } .-1 }
    fn f3(
        ref mut a @ [
// { dg-error ".E0499." "" { target *-*-* } .-1 }
            [ref b @ .., _],
            [_, ref mut mid @ ..],
            ..,
            [..],
        ] : [[U; 4]; 5]
    ) {}
    fn f4_also_moved(ref mut a @ ref mut b @ c: U) {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
// { dg-error ".E0382." "" { target *-*-* } .-3 }

    let ref mut a @ ref mut b = U;
// { dg-error ".E0499." "" { target *-*-* } .-1 }
    drop(a);
    let ref mut a @ ref mut b = U;
// { dg-error ".E0499." "" { target *-*-* } .-1 }
// { dg-error ".E0499." "" { target *-*-* } .-2 }
    drop(b);
    let ref mut a @ ref mut b = U;
// { dg-error ".E0499." "" { target *-*-* } .-1 }

    let ref mut a @ ref mut b = U;
// { dg-error ".E0499." "" { target *-*-* } .-1 }
    *a = U;
    let ref mut a @ ref mut b = U;
// { dg-error ".E0499." "" { target *-*-* } .-1 }
// { dg-error ".E0499." "" { target *-*-* } .-2 }
    *b = U;

    let ref mut a @ (
// { dg-error ".E0499." "" { target *-*-* } .-1 }
        ref mut b,
        [
            ref mut c,
            ref mut d,
            ref e,
        ]
    ) = (U, [U, U, U]);

    let ref mut a @ (
// { dg-error ".E0499." "" { target *-*-* } .-1 }
            ref mut b,
            [
                ref mut c,
                ref mut d,
                ref e,
            ]
        ) = (u(), [u(), u(), u()]);

    let a @ (ref mut b, ref mut c) = (U, U);
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    let mut val = (U, [U, U]);
    let a @ (b, [c, d]) = &mut val; // Same as ^--
// { dg-error ".E0382." "" { target *-*-* } .-1 }

    let a @ &mut ref mut b = &mut U;
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    let a @ &mut (ref mut b, ref mut c) = &mut (U, U);
// { dg-error ".E0382." "" { target *-*-* } .-1 }

    match Ok(U) {
        ref mut a @ Ok(ref mut b) | ref mut a @ Err(ref mut b) => {
// { dg-error ".E0499." "" { target *-*-* } .-1 }
// { dg-error ".E0499." "" { target *-*-* } .-2 }
        }
    }
    match Ok(U) {
        ref mut a @ Ok(ref mut b) | ref mut a @ Err(ref mut b) => {
// { dg-error ".E0499." "" { target *-*-* } .-1 }
// { dg-error ".E0499." "" { target *-*-* } .-2 }
            *b = U;
        }
    }
    match Ok(U) {
        ref mut a @ Ok(ref mut b) | ref mut a @ Err(ref mut b) => {
// { dg-error ".E0499." "" { target *-*-* } .-1 }
// { dg-error ".E0499." "" { target *-*-* } .-2 }
// { dg-error ".E0499." "" { target *-*-* } .-3 }
// { dg-error ".E0499." "" { target *-*-* } .-4 }
            *a = Err(U);

            // FIXME: The binding name value used above makes for problematic diagnostics.
            // Resolve that somehow...
        }
    }
    match Ok(U) {
        ref mut a @ Ok(ref mut b) | ref mut a @ Err(ref mut b) => {
// { dg-error ".E0499." "" { target *-*-* } .-1 }
// { dg-error ".E0499." "" { target *-*-* } .-2 }
// { dg-error ".E0499." "" { target *-*-* } .-3 }
// { dg-error ".E0499." "" { target *-*-* } .-4 }
            drop(a);
        }
    }
}

