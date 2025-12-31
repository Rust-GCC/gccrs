// Check that a constant-evaluation underflow highlights the correct
// spot (where the underflow occurred), while also providing the
// overall context for what caused the evaluation.

const ONE: usize = 1;
const TWO: usize = 2;
const LEN: usize = ONE - TWO;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    let a: [i8; LEN] = unimplemented!();
// { dg-error ".E0080." "" { target *-*-* } .-1 }
}

