// edition:2018
#![deny(unreachable_code)]

async fn foo() {
    return; bar().await;
// { dg-error "" "" { target *-*-* } .-1 }
}

async fn bar() {
}

fn main() { }

