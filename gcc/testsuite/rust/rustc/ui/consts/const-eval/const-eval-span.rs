// Check that error in constant evaluation of enum discriminant
// provides the context for what caused the evaluation.

struct S(i32);

const CONSTANT: S = S(0);

enum E {
    V = CONSTANT,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn main() {}

