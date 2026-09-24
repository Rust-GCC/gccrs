fn foo<T: Iterator>() {
    T::Item; // { dg-error ".E0599." "" { target *-*-* } }
}

fn main() { }

