#![feature(negative_impls)]

use std::marker::Send;

struct Foo {
    a: isize,
}

impl !Send for Foo {}

fn bar<T: Send>(_: T) {}

fn main() {
    let x = Foo { a: 5 };
    bar(x);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

