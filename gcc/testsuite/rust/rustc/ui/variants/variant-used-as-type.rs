// Test error message when enum variants are used as types


// issue 21225
enum Ty {
    A,
    B(Ty::A),
// { dg-error ".E0573." "" { target *-*-* } .-1 }
}


// issue 19197
enum E {
    A
}

impl E::A {}
// { dg-error ".E0573." "" { target *-*-* } .-1 }

fn main() {}

