pub trait Foo {
    type Type;
}

pub struct Bar<T>(<Self as Foo>::Type) where Self: ;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {}

