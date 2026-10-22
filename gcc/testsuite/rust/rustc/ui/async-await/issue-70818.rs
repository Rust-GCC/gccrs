// edition:2018

use std::future::Future;
fn foo<T: Send, U>(ty: T, ty1: U) -> impl Future<Output = (T, U)> + Send {
// { dg-error "" "" { target *-*-* } .-1 }
    async { (ty, ty1) }
}

fn main() {}

