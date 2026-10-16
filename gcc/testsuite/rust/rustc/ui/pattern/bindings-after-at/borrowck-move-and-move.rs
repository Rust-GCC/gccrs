// Test that moving on both sides of an `@` pattern is not allowed.

#![feature(bindings_after_at)]

fn main() {
    struct U; // Not copy!

    // Prevent promotion:
    fn u() -> U {
        U
    }

    let a @ b = U; // { dg-error ".E0382." "" { target *-*-* } }

    let a @ (b, c) = (U, U); // { dg-error ".E0382." "" { target *-*-* } }

    let a @ (b, c) = (u(), u()); // { dg-error ".E0382." "" { target *-*-* } }

    match Ok(U) {
        a @ Ok(b) | a @ Err(b) => {} // { dg-error ".E0382." "" { target *-*-* } }
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    }

    fn fun(a @ b: U) {} // { dg-error ".E0382." "" { target *-*-* } }

    match [u(), u(), u(), u()] {
        xs @ [a, .., b] => {} // { dg-error ".E0382." "" { target *-*-* } }
    }

    match [u(), u(), u(), u()] {
        xs @ [_, ys @ .., _] => {} // { dg-error ".E0382." "" { target *-*-* } }
    }
}

