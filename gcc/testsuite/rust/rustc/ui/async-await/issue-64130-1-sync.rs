#![feature(negative_impls)]
// edition:2018

// This tests the the specialized async-await-specific error when futures don't implement an
// auto trait (which is specifically Sync) due to some type that was captured.

struct Foo;

impl !Sync for Foo {}

fn is_sync<T: Sync>(t: T) { }

async fn bar() {
    let x = Foo;
    baz().await;
}

async fn baz() { }

fn main() {
    is_sync(bar());
// { dg-error "" "" { target *-*-* } .-1 }
}

