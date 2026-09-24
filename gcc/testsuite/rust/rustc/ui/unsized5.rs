// Test `?Sized` types not allowed in fields (except the last one).

struct S1<X: ?Sized> {
    f1: X,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    f2: isize,
}
struct S2<X: ?Sized> {
    f: isize,
    g: X,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    h: isize,
}
struct S3 {
    f: str,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    g: [usize]
}
struct S4 {
    f: [u8],
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    g: usize
}
enum E<X: ?Sized> {
    V1(X, isize),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}
enum F<X: ?Sized> {
    V2{f1: X, f: isize},
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

pub fn main() {
}

