// edition:2018

// Regression test for issue #62382.

use std::future::Future;

fn get_future() -> impl Future<Output = ()> {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    panic!()
}

async fn foo() {
    let a; // { dg-error ".E0698." "" { target *-*-* } }
    get_future().await;
}

fn main() {}

