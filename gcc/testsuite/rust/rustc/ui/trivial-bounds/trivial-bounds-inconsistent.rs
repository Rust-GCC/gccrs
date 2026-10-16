// run-pass

// Check that tautalogically false bounds are accepted, and are used
// in type inference.
#![feature(trivial_bounds)]
#![allow(unused)]

pub trait Foo {
    fn test(&self);
}

fn generic_function<X: Foo>(x: X) {}

enum E where i32: Foo { V } // { dg-warning "" "" { target *-*-* } }

struct S where i32: Foo; // { dg-warning "" "" { target *-*-* } }

trait T where i32: Foo {} // { dg-warning "" "" { target *-*-* } }

union U where i32: Foo { f: i32 } // { dg-warning "" "" { target *-*-* } }

type Y where i32: Foo = ();
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

impl Foo for () where i32: Foo { // { dg-warning "" "" { target *-*-* } }
    fn test(&self) {
        3i32.test();
        Foo::test(&4i32);
        generic_function(5i32);
    }
}

fn f() where i32: Foo { // { dg-warning "" "" { target *-*-* } }
    let s = S;
    3i32.test();
    Foo::test(&4i32);
    generic_function(5i32);
}

fn g() where &'static str: Foo { // { dg-warning "" "" { target *-*-* } }
    "Foo".test();
    Foo::test(&"Foo");
    generic_function("Foo");
}

trait A {}

impl A for i32 {}

struct Dst<X: ?Sized> {
    x: X,
}

struct TwoStrs(str, str) where str: Sized; // { dg-warning "" "" { target *-*-* } }

fn unsized_local() where for<'a> Dst<dyn A + 'a>: Sized { // { dg-warning "" "" { target *-*-* } }
    let x: Dst<dyn A> = *(Box::new(Dst { x: 1 }) as Box<Dst<dyn A>>);
}

fn return_str() -> str where str: Sized { // { dg-warning "" "" { target *-*-* } }
    *"Sized".to_string().into_boxed_str()
}

fn use_op(s: String) -> String where String: ::std::ops::Neg<Output=String> {
// { dg-warning "" "" { target *-*-* } .-1 }
    -s
}

fn use_for() where i32: Iterator { // { dg-warning "" "" { target *-*-* } }
    for _ in 2i32 {}
}

fn main() {}

