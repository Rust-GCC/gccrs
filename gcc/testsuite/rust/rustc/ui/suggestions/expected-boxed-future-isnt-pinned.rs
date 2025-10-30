// edition:2018
#![allow(dead_code)]
use std::future::Future;
use std::pin::Pin;

type BoxFuture<'a, T> = Pin<Box<dyn Future<Output = T> + Send + 'a>>;
//   ^^^^^^^^^ This would come from the `futures` crate in real code.

fn foo<F: Future<Output=i32> + Send + 'static>(x: F) -> BoxFuture<'static, i32> {
    // We could instead use an `async` block, but this way we have no std spans.
    x // { dg-error ".E0308." "" { target *-*-* } }
}

// This case is still subpar:
// `Pin::new(x)`: store this in the heap by calling `Box::new`: `Box::new(x)`
// Should suggest changing the code from `Pin::new` to `Box::pin`.
fn bar<F: Future<Output=i32> + Send + 'static>(x: F) -> BoxFuture<'static, i32> {
    Box::new(x) // { dg-error ".E0308." "" { target *-*-* } }
}

fn baz<F: Future<Output=i32> + Send + 'static>(x: F) -> BoxFuture<'static, i32> {
    Pin::new(x) // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn qux<F: Future<Output=i32> + Send + 'static>(x: F) -> BoxFuture<'static, i32> {
    Pin::new(Box::new(x)) // { dg-error ".E0277." "" { target *-*-* } }
}

fn zap() -> BoxFuture<'static, i32> {
    async { // { dg-error ".E0308." "" { target *-*-* } }
        42
    }
}

fn main() {}

