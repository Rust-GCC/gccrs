use std::pin::Pin;
use std::future::Future;

fn main() {}

fn await_on_struct_missing() {
    struct S;
    let x = S;
    x.await;
// { dg-error ".E0609." "" { target *-*-* } .-1 }
// { dg-note ".E0609." "" { target *-*-* } .-2 }
// { dg-note ".E0609." "" { target *-*-* } .-3 }
// { help ".E0609." "" { target *-*-* } .-4 }
// { dg-note ".E0609." "" { target *-*-* } .-5 }
}

fn await_on_struct_similar() {
    struct S {
        awai: u8,
    }
    let x = S { awai: 42 };
    x.await;
// { dg-error ".E0609." "" { target *-*-* } .-1 }
// { help ".E0609." "" { target *-*-* } .-2 }
// { dg-note ".E0609." "" { target *-*-* } .-3 }
// { help ".E0609." "" { target *-*-* } .-4 }
// { dg-note ".E0609." "" { target *-*-* } .-5 }
}

fn await_on_63533(x: Pin<&mut dyn Future<Output = ()>>) {
    x.await;
// { dg-error ".E0609." "" { target *-*-* } .-1 }
// { dg-note ".E0609." "" { target *-*-* } .-2 }
// { dg-note ".E0609." "" { target *-*-* } .-3 }
// { help ".E0609." "" { target *-*-* } .-4 }
// { dg-note ".E0609." "" { target *-*-* } .-5 }
}

fn await_on_apit(x: impl Future<Output = ()>) {
    x.await;
// { dg-error ".E0609." "" { target *-*-* } .-1 }
// { dg-note ".E0609." "" { target *-*-* } .-2 }
// { help ".E0609." "" { target *-*-* } .-3 }
// { dg-note ".E0609." "" { target *-*-* } .-4 }
}

