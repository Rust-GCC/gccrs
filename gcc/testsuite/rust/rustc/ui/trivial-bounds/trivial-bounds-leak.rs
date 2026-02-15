// Check that false bounds don't leak
#![feature(trivial_bounds)]

pub trait Foo {
    fn test(&self);
}

fn return_str() -> str where str: Sized {
    *"Sized".to_string().into_boxed_str()
}

fn cant_return_str() -> str { // { dg-error ".E0277." "" { target *-*-* } }
    *"Sized".to_string().into_boxed_str()
}

fn my_function() where i32: Foo
{
    3i32.test();
    Foo::test(&4i32);
    generic_function(5i32);
}

fn foo() {
    3i32.test(); // { dg-error ".E0599." "" { target *-*-* } }
    Foo::test(&4i32); // { dg-error ".E0277." "" { target *-*-* } }
    generic_function(5i32); // { dg-error ".E0277." "" { target *-*-* } }
}

fn generic_function<T: Foo>(t: T) {}

fn main() {}

