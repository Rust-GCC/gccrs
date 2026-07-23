// issue #36286

struct S<T: Clone> { a: T }

struct NoClone;
type A = S<NoClone>;

fn main() {
    let s = A { a: NoClone };
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

