#![feature(associated_type_defaults)]

trait Foo { type T; }
trait Bar {
    type Foo: Foo;
    type FooT = <<Self as Bar>::Foo>::T; // { dg-error ".E0223." "" { target *-*-* } }
}

fn main() {}

