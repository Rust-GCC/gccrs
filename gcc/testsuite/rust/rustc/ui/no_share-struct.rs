#![feature(negative_impls)]

use std::marker::Sync;

struct Foo { a: isize }
impl !Sync for Foo {}

fn bar<T: Sync>(_: T) {}

fn main() {
    let x = Foo { a: 5 };
    bar(x);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

