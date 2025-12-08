fn main() {
    struct U;

    // A tuple is a "non-reference pattern".
    // A `mut` binding pattern resets the binding mode to by-value.

    let p = (U, U);
    let (a, mut b) = &p;
// { dg-error ".E0507." "" { target *-*-* } .-1 }

    let mut p = (U, U);
    let (a, mut b) = &mut p;
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

