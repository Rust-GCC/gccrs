use std::ops::Deref;

trait PointerFamily<U> {
    type Pointer<T>: Deref<Target = T>;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    type Pointer2<T>: Deref<Target = T> where T: Clone, U: Clone;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-error ".E0658." "" { target *-*-* } .-2 }
}

struct Foo;

impl PointerFamily<u32> for Foo {
    type Pointer<Usize> = Box<Usize>;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    type Pointer2<U32> = Box<U32>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

trait Bar {
    type Assoc where Self: Sized;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

impl Bar for Foo {
    type Assoc where Self: Sized = Foo;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

fn main() {}

