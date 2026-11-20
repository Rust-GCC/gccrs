// edition:2018

#![feature(async_closure)]

async fn print_dur() {}

fn main() {
    (async || 2333)().await;
// { dg-error ".E0728." "" { target *-*-* } .-1 }
}

