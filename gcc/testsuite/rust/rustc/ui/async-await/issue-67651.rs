// edition:2018

trait From {
    fn from();
}

impl From for () {
    fn from() {}
}

impl From for () {
// { dg-error ".E0119." "" { target *-*-* } .-1 }
    fn from() {}
}

fn bar() -> impl core::future::Future<Output = ()> {
    async move { From::from() }
}

fn main() {}

