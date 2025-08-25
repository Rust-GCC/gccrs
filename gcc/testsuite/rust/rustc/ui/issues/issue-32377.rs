// normalize-stderr-test "\d+ bits" -> "N bits"

use std::mem;
use std::marker::PhantomData;

trait Foo {
    type Error;
}

struct Bar<U: Foo> {
    stream: PhantomData<U::Error>,
}

fn foo<U: Foo>(x: [usize; 2]) -> Bar<U> {
    unsafe { mem::transmute(x) }
// { dg-error ".E0512." "" { target *-*-* } .-1 }
}

fn main() {}

