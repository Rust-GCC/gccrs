type Foo<T,T> = Option<T>;
// { dg-error ".E0403." "" { target *-*-* } .-1 }

struct Bar<T,T>(T);
// { dg-error ".E0403." "" { target *-*-* } .-1 }

struct Baz<T,T> {
// { dg-error ".E0403." "" { target *-*-* } .-1 }
    x: T,
}

enum Boo<T,T> {
// { dg-error ".E0403." "" { target *-*-* } .-1 }
    A(T),
    B,
}

fn quux<T,T>(x: T) {}
// { dg-error ".E0403." "" { target *-*-* } .-1 }

trait Qux<T,T> {}
// { dg-error ".E0403." "" { target *-*-* } .-1 }

impl<T,T> Qux<T,T> for Option<T> {}
// { dg-error ".E0207." "" { target *-*-* } .-1 }
// { dg-error ".E0207." "" { target *-*-* } .-2 }

fn main() {
}

