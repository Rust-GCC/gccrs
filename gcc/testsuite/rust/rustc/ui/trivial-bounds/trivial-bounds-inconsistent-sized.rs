// run-pass
// Check tautalogically false `Sized` bounds
#![feature(trivial_bounds)]
#![allow(unused)]

trait A {}

impl A for i32 {}

struct T<X: ?Sized> {
    x: X,
}

struct S(str, str) where str: Sized;
// { dg-warning "" "" { target *-*-* } .-1 }

fn unsized_local() where for<'a> T<dyn A + 'a>: Sized {
// { dg-warning "" "" { target *-*-* } .-1 }
    let x: T<dyn A> = *(Box::new(T { x: 1 }) as Box<T<dyn A>>);
}

fn return_str() -> str where str: Sized {
// { dg-warning "" "" { target *-*-* } .-1 }
    *"Sized".to_string().into_boxed_str()
}

fn main() {}

