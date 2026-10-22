#![allow(unused)]
#![allow(type_alias_bounds)]

pub trait Foo {
    fn test(&self);
}

fn generic_function<X: Foo>(x: X) {}

enum E where i32: Foo { V } // { dg-error ".E0277." "" { target *-*-* } }

struct S where i32: Foo; // { dg-error ".E0277." "" { target *-*-* } }

trait T where i32: Foo {} // { dg-error ".E0277." "" { target *-*-* } }

union U where i32: Foo { f: i32 } // { dg-error ".E0277." "" { target *-*-* } }

type Y where i32: Foo = (); // OK - bound is ignored

impl Foo for () where i32: Foo { // { dg-error ".E0277." "" { target *-*-* } }
    fn test(&self) {
        3i32.test();
        Foo::test(&4i32);
        generic_function(5i32);
    }
}

fn f() where i32: Foo // { dg-error ".E0277." "" { target *-*-* } }
{
    let s = S;
    3i32.test();
    Foo::test(&4i32);
    generic_function(5i32);
}

fn use_op(s: String) -> String where String: ::std::ops::Neg<Output=String> { // { dg-error ".E0277." "" { target *-*-* } }
    -s
}

fn use_for() where i32: Iterator { // { dg-error ".E0277." "" { target *-*-* } }
    for _ in 2i32 {}
}

trait A {}

impl A for i32 {}

struct Dst<X: ?Sized> {
    x: X,
}

struct TwoStrs(str, str) where str: Sized; // { dg-error ".E0277." "" { target *-*-* } }


fn unsized_local() where Dst<dyn A>: Sized { // { dg-error ".E0277." "" { target *-*-* } }
    let x: Dst<dyn A> = *(Box::new(Dst { x: 1 }) as Box<Dst<dyn A>>);
}

fn return_str() -> str where str: Sized { // { dg-error ".E0277." "" { target *-*-* } }
    *"Sized".to_string().into_boxed_str()
}

// This is currently accepted because the function pointer isn't
// considered global.
fn global_hr(x: fn(&())) where fn(&()): Foo { // OK
    x.test();
}

fn main() {}

