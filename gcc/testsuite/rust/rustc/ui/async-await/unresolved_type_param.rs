// Provoke an unresolved type error (T).
// Error message should pinpoint the type parameter T as needing to be bound
// (rather than give a general error message)
// edition:2018

async fn bar<T>() -> () {}

async fn foo() {
    bar().await;
// { dg-error ".E0698." "" { target *-*-* } .-1 }
// { dg-error ".E0698." "" { target *-*-* } .-2 }
// { dg-error ".E0698." "" { target *-*-* } .-3 }
// { dg-note ".E0698." "" { target *-*-* } .-4 }
// { dg-note ".E0698." "" { target *-*-* } .-5 }
// { dg-note ".E0698." "" { target *-*-* } .-6 }
// { dg-note ".E0698." "" { target *-*-* } .-7 }
// { dg-note ".E0698." "" { target *-*-* } .-8 }
// { dg-note ".E0698." "" { target *-*-* } .-9 }
// { dg-note ".E0698." "" { target *-*-* } .-10 }
// { dg-note ".E0698." "" { target *-*-* } .-11 }
// { dg-note ".E0698." "" { target *-*-* } .-12 }
}
fn main() {}

