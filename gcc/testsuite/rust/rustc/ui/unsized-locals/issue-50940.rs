fn main() {
    struct A<X: ?Sized>(X);
    A as fn(str) -> A<str>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

