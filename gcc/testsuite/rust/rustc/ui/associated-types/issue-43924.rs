#![feature(associated_type_defaults)]

// This used to cause an ICE because assoc. type defaults weren't properly
// type-checked.

trait Foo<T: Default + ToString> {
    type Out: Default + ToString + ?Sized = dyn ToString;  // { dg-error ".E0277." "" { target *-*-* } }
}

impl Foo<u32> for () {}
impl Foo<u64> for () {}

fn main() {
    assert_eq!(<() as Foo<u32>>::Out::default().to_string(), "false");
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

