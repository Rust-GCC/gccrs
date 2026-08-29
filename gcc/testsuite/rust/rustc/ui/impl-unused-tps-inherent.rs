struct MyType;

struct MyType1<T>(T);

trait Bar {
    type Out;
}

impl<T> MyType {
// { dg-error ".E0207." "" { target *-*-* } .-1 }
}

impl<T> MyType1<T> {
    // OK, T is used in `Foo<T>`.
}

impl<T,U> MyType1<T> {
// { dg-error ".E0207." "" { target *-*-* } .-1 }
}

impl<T,U> MyType1<T> where T: Bar<Out=U> {
    // OK, T is used in `Foo<T>`.
}

fn main() { }

