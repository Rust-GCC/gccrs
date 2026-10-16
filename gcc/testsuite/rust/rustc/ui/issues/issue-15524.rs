const N: isize = 1;

enum Foo {
    A = 1,
    B = 1,
// { dg-error ".E0081." "" { target *-*-* } .-1 }
    C = 0,
    D,
// { dg-error ".E0081." "" { target *-*-* } .-1 }

    E = N,
// { dg-error ".E0081." "" { target *-*-* } .-1 }

}

fn main() {}

