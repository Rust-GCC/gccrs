// edition:2018
#![feature(async_closure)]
use std::future::Future;

async fn foo() {}

fn bar(f: impl Future<Output=()>) {}

fn main() {
    bar(foo); // { dg-error ".E0277." "" { target *-*-* } }
    let async_closure = async || ();
    bar(async_closure); // { dg-error ".E0277." "" { target *-*-* } }
}

