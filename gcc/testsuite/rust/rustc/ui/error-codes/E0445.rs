trait Foo {
    fn dummy(&self) { }
}

pub trait Bar : Foo {}
// { dg-error ".E0445." "" { target *-*-* } .-1 }
pub struct Bar2<T: Foo>(pub T);
// { dg-error ".E0445." "" { target *-*-* } .-1 }
pub fn foo<T: Foo> (t: T) {}
// { dg-error ".E0445." "" { target *-*-* } .-1 }

fn main() {}

