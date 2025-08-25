use std::marker;

struct Foo<T,U>(T, marker::PhantomData<U>);

fn main() {
    match Foo(1.1, marker::PhantomData) {
        1 => {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
    }

}

