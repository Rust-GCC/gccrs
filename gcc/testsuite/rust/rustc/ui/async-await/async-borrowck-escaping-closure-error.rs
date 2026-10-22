// edition:2018
#![feature(async_closure)]
fn foo() -> Box<dyn std::future::Future<Output = u32>> {
    let x = 0u32;
    Box::new((async || x)())
// { dg-error ".E0373." "" { target *-*-* } .-1 }
}

fn main() {
}

