// Check that a constant-evaluation underflow highlights the correct
// spot (where the underflow occurred).

const ONE: usize = 1;
const TWO: usize = 2;

fn main() {
    let a: [i8; ONE - TWO] = unimplemented!();
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }
}

