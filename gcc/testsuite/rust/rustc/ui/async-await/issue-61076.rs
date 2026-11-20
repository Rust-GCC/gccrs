// edition:2018

use core::future::Future;
use core::pin::Pin;
use core::task::{Context, Poll};

struct T;

struct Tuple(i32);

struct Struct {
    a: i32
}

impl Struct {
    fn method(&self) {}
}

impl Future for Struct {
    type Output = Struct;
    fn poll(self: Pin<&mut Self>, _: &mut Context<'_>) -> Poll<Self::Output> { Poll::Pending }
}

impl Future for Tuple {
    type Output = Tuple;
    fn poll(self: Pin<&mut Self>, _: &mut Context<'_>) -> Poll<Self::Output> { Poll::Pending }
}

impl Future for T {
    type Output = Result<(), ()>;

    fn poll(self: Pin<&mut Self>, _: &mut Context<'_>) -> Poll<Self::Output> {
        Poll::Pending
    }
}

async fn foo() -> Result<(), ()> {
    Ok(())
}

async fn bar() -> Result<(), ()> {
    foo()?; // { dg-error ".E0277." "" { target *-*-* } }
// { dg-note ".E0277." "" { target *-*-* } .-1 }
// { help ".E0277." "" { target *-*-* } .-2 }
// { dg-note ".E0277." "" { target *-*-* } .-3 }
// { help ".E0277." "" { target *-*-* } .-4 }
// { dg-note ".E0277." "" { target *-*-* } .-5 }
// { dg-note ".E0277." "" { target *-*-* } .-6 }
// { dg-note ".E0277." "" { target *-*-* } .-7 }
// { dg-note ".E0277." "" { target *-*-* } .-8 }
    Ok(())
}

async fn struct_() -> Struct {
    Struct { a: 1 }
}

async fn tuple() -> Tuple {
// { dg-note "" "" { target *-*-* } .-1 }
    Tuple(1i32)
}

async fn baz() -> Result<(), ()> {
    let t = T;
    t?; // { dg-error ".E0277." "" { target *-*-* } }
// { dg-note ".E0277." "" { target *-*-* } .-1 }
// { help ".E0277." "" { target *-*-* } .-2 }
// { dg-note ".E0277." "" { target *-*-* } .-3 }
// { help ".E0277." "" { target *-*-* } .-4 }
// { dg-note ".E0277." "" { target *-*-* } .-5 }
// { dg-note ".E0277." "" { target *-*-* } .-6 }
// { dg-note ".E0277." "" { target *-*-* } .-7 }
// { dg-note ".E0277." "" { target *-*-* } .-8 }


    let _: i32 = tuple().0; // { dg-error ".E0609." "" { target *-*-* } }
// { help ".E0609." "" { target *-*-* } .-1 }
// { dg-note ".E0609." "" { target *-*-* } .-2 }

    let _: i32 = struct_().a; // { dg-error ".E0609." "" { target *-*-* } }
// { help ".E0609." "" { target *-*-* } .-1 }
// { dg-note ".E0609." "" { target *-*-* } .-2 }

    struct_().method(); // { dg-error ".E0599." "" { target *-*-* } }
// { dg-note ".E0599." "" { target *-*-* } .-1 }
// { help ".E0599." "" { target *-*-* } .-2 }
    Ok(())
}

async fn match_() {
    match tuple() { // { help "" "" { target *-*-* } }
        Tuple(_) => {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-note ".E0308." "" { target *-*-* } .-1 }
// { dg-note ".E0308." "" { target *-*-* } .-2 }
    }
}

fn main() {}

