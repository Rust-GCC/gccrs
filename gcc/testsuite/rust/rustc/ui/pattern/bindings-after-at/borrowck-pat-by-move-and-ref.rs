// Test that `ref mut? @ pat_with_by_move_bindings` is prevented.

#![feature(bindings_after_at)]

fn main() {
    struct U;

    // Prevent promotion.
    fn u() -> U {
        U
    }

    fn f1(ref a @ b: U) {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
    fn f2(ref a @ (ref b @ mut c, ref d @ e): (U, U)) {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
// { dg-error ".E0382." "" { target *-*-* } .-3 }
// { dg-error ".E0382." "" { target *-*-* } .-4 }
// { dg-error ".E0382." "" { target *-*-* } .-5 }
    fn f3(ref mut a @ [b, mut c]: [U; 2]) {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }

    let ref a @ b = U;
// { dg-error "" "" { target *-*-* } .-1 }
    let ref a @ (ref b @ mut c, ref d @ e) = (U, U);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
    let ref mut a @ [b, mut c] = [U, U];
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
    let ref a @ b = u();
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
    let ref a @ (ref b @ mut c, ref d @ e) = (u(), u());
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
// { dg-error ".E0382." "" { target *-*-* } .-3 }
// { dg-error ".E0382." "" { target *-*-* } .-4 }
// { dg-error ".E0382." "" { target *-*-* } .-5 }
    let ref mut a @ [b, mut c] = [u(), u()];
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }

    match Some(U) {
        ref a @ Some(b) => {}
// { dg-error "" "" { target *-*-* } .-1 }
        None => {}
    }
    match Some((U, U)) {
        ref a @ Some((ref b @ mut c, ref d @ e)) => {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
        None => {}
    }
    match Some([U, U]) {
        ref mut a @ Some([b, mut c]) => {}
// { dg-error "" "" { target *-*-* } .-1 }
        None => {}
    }
    match Some(u()) {
        ref a @ Some(b) => {}
// { dg-error "" "" { target *-*-* } .-1 }
        None => {}
    }
    match Some((u(), u())) {
        ref a @ Some((ref b @ mut c, ref d @ e)) => {}
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
// { dg-error ".E0382." "" { target *-*-* } .-3 }
// { dg-error ".E0382." "" { target *-*-* } .-4 }
// { dg-error ".E0382." "" { target *-*-* } .-5 }
        None => {}
    }
    match Some([u(), u()]) {
        ref mut a @ Some([b, mut c]) => {}
// { dg-error "" "" { target *-*-* } .-1 }
        None => {}
    }
}

