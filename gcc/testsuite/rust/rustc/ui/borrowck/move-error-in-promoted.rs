// Regression test for #70934

fn f() {
    const C: [S2; 1] = [S2];
    let _ = S1(C[0]).clone();
// { dg-error ".E0508." "" { target *-*-* } .-1 }
}

#[derive(Clone)]
struct S1(S2);

#[derive(Clone)]
struct S2;

fn main() {
    f();
}

