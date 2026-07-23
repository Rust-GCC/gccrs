struct T;

fn main() {
    T::new();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

