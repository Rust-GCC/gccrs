enum Enum {
    P = 3,
    X = 3,
// { dg-error ".E0081." "" { target *-*-* } .-1 }
    Y = 5
}

fn main() {
}

