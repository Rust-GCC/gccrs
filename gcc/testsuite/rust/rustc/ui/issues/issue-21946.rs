trait Foo {
    type A;
}

struct FooStruct;

impl Foo for FooStruct {
    type A = <FooStruct as Foo>::A;
// { dg-error ".E0275." "" { target *-*-* } .-1 }
}

fn main() {}

