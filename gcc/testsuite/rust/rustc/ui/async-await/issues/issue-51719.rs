// edition:2018
//
// Tests that the .await syntax can't be used to make a generator

async fn foo() {}

fn make_generator() {
    let _gen = || foo().await;
// { dg-error ".E0728." "" { target *-*-* } .-1 }
}

fn main() {}

