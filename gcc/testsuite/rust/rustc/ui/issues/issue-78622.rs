#![crate_type = "lib"]

struct S;
fn f() {
    S::A::<f> {}
// { dg-error ".E0223." "" { target *-*-* } .-1 }
}

