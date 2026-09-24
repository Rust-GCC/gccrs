type A = rustfmt; // { dg-error ".E0573." "" { target *-*-* } }
type B = rustfmt::skip; // { dg-error ".E0573." "" { target *-*-* } }

#[derive(rustfmt)] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
struct S;

// Interpreted as an unstable custom attribute
#[rustfmt] // { dg-error "" "" { target *-*-* } }
fn check() {}

#[rustfmt::skip] // OK
fn main() {
    rustfmt; // { dg-error ".E0423." "" { target *-*-* } }
    rustfmt!(); // { dg-error "" "" { target *-*-* } }

    rustfmt::skip; // { dg-error ".E0423." "" { target *-*-* } }
}

