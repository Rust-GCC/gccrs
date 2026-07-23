// gate-test-associated_type_defaults

trait Foo {
    type Bar = u8; // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {}

