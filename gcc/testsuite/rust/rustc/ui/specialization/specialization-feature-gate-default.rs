// Check that specialization must be ungated to use the `default` keyword

// gate-test-specialization

trait Foo {
    fn foo(&self);
}

impl<T> Foo for T {
    default fn foo(&self) {} // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {}

