// Check that writing an overlapping impl is not allow unless specialization is ungated.

// gate-test-specialization

trait Foo {
    fn foo(&self);
}

impl<T> Foo for T {
    fn foo(&self) {}
}

impl Foo for u8 { // { dg-error ".E0119." "" { target *-*-* } }
    fn foo(&self) {}
}

fn main() {}

