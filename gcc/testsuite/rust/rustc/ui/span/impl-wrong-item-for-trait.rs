use std::fmt::Debug;

trait Foo {
    fn bar(&self);
    const MY_CONST: u32;
}

pub struct FooConstForMethod;

impl Foo for FooConstForMethod {
// { dg-error ".E0046." "" { target *-*-* } .-1 }
    const bar: u64 = 1;
// { dg-error ".E0323." "" { target *-*-* } .-1 }
    const MY_CONST: u32 = 1;
}

pub struct FooMethodForConst;

impl Foo for FooMethodForConst {
// { dg-error ".E0046." "" { target *-*-* } .-1 }
    fn bar(&self) {}
    fn MY_CONST() {}
// { dg-error ".E0324." "" { target *-*-* } .-1 }
}

pub struct FooTypeForMethod;

impl Foo for FooTypeForMethod {
// { dg-error ".E0046." "" { target *-*-* } .-1 }
    type bar = u64;
// { dg-error ".E0325." "" { target *-*-* } .-1 }
// { dg-error ".E0325." "" { target *-*-* } .-2 }
    const MY_CONST: u32 = 1;
}

impl Debug for FooTypeForMethod {
}
// { dg-error ".E0046." "" { target *-*-* } .-2 }

fn main () {}

