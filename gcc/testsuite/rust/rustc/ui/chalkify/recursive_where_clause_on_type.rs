// FIXME(chalk): should fail, see comments
// check-fail
// compile-flags: -Z chalk

#![feature(trivial_bounds)]

trait Bar {
    fn foo();
}
trait Foo: Bar { }

struct S where S: Foo;

impl Foo for S {
}

fn bar<T: Bar>() {
    T::foo();
}

fn foo<T: Foo>() {
    bar::<T>()
}

fn main() {
    // For some reason, the error is duplicated...

    foo::<S>() // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

