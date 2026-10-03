// Check that specialization must be ungated to use the `default` keyword

trait Foo {
    fn foo(&self);
}

default impl<T> Foo for T { // { dg-error ".E0658." "" { target *-*-* } }
    fn foo(&self) {}
}

fn main() {}

