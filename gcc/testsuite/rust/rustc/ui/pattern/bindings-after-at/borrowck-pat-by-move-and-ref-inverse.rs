// Test that `by_move_binding @ pat_with_by_ref_bindings` is prevented.

#![feature(bindings_after_at)]

fn main() {
    struct U;

    // Prevent promotion.
    fn u() -> U {
        U
    }

    fn f1(a @ ref b: U) {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }

    fn f2(mut a @ (b @ ref c, mut d @ ref e): (U, U)) {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
// { dg-error ".E0382." "" { target *-*-* } .-3 }
// { dg-error ".E0382." "" { target *-*-* } .-4 }
    fn f3(a @ [ref mut b, ref c]: [U; 2]) {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }

    let a @ ref b = U;
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    let a @ (mut b @ ref mut c, d @ ref e) = (U, U);
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
// { dg-error ".E0382." "" { target *-*-* } .-3 }
// { dg-error ".E0382." "" { target *-*-* } .-4 }
    let a @ [ref mut b, ref c] = [U, U];
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    let a @ ref b = u();
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    let a @ (mut b @ ref mut c, d @ ref e) = (u(), u());
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
// { dg-error ".E0382." "" { target *-*-* } .-3 }
// { dg-error ".E0382." "" { target *-*-* } .-4 }
    let a @ [ref mut b, ref c] = [u(), u()];
// { dg-error ".E0382." "" { target *-*-* } .-1 }

    match Some(U) {
        a @ Some(ref b) => {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
        None => {}
    }
    match Some((U, U)) {
        a @ Some((mut b @ ref mut c, d @ ref e)) => {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
// { dg-error ".E0382." "" { target *-*-* } .-3 }
// { dg-error ".E0382." "" { target *-*-* } .-4 }
        None => {}
    }
    match Some([U, U]) {
        mut a @ Some([ref b, ref mut c]) => {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
        None => {}
    }
    match Some(u()) {
        a @ Some(ref b) => {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
        None => {}
    }
    match Some((u(), u())) {
        a @ Some((mut b @ ref mut c, d @ ref e)) => {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
// { dg-error ".E0382." "" { target *-*-* } .-3 }
// { dg-error ".E0382." "" { target *-*-* } .-4 }
        None => {}
    }
    match Some([u(), u()]) {
        mut a @ Some([ref b, ref mut c]) => {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
        None => {}
    }
}

