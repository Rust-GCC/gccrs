// Ensure that we get an error and not an ICE for this problematic case.
struct Foo<T = Option<U>, U = bool>(T, U);
// { dg-error ".E0128." "" { target *-*-* } .-1 }
fn main() {
    let x: Foo;
}

