// Tests that the use of uninitialized variable in assignment operator
// expression is detected.

pub fn main() {
    let x: isize;
    x += 1; // { dg-error ".E0381." "" { target *-*-* } }

    let x: isize;
    x -= 1; // { dg-error ".E0381." "" { target *-*-* } }

    let x: isize;
    x *= 1; // { dg-error ".E0381." "" { target *-*-* } }

    let x: isize;
    x /= 1; // { dg-error ".E0381." "" { target *-*-* } }

    let x: isize;
    x %= 1; // { dg-error ".E0381." "" { target *-*-* } }

    let x: isize;
    x ^= 1; // { dg-error ".E0381." "" { target *-*-* } }

    let x: isize;
    x &= 1; // { dg-error ".E0381." "" { target *-*-* } }

    let x: isize;
    x |= 1; // { dg-error ".E0381." "" { target *-*-* } }

    let x: isize;
    x <<= 1;    // { dg-error ".E0381." "" { target *-*-* } }

    let x: isize;
    x >>= 1;    // { dg-error ".E0381." "" { target *-*-* } }
}

