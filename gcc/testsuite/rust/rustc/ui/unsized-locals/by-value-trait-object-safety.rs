#![feature(unsized_locals)]
// { dg-warning "" "" { target *-*-* } .-1 }

pub trait Foo {
    fn foo(self) -> String
    where
        Self: Sized;
}

struct A;

impl Foo for A {
    fn foo(self) -> String {
        format!("hello")
    }
}

fn main() {
    let x = *(Box::new(A) as Box<dyn Foo>);
    x.foo();
// { dg-error "" "" { target *-*-* } .-1 }
}

