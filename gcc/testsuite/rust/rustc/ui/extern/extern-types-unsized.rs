// Make sure extern types are !Sized.

#![feature(extern_types)]

extern {
    type A;
}

struct Foo {
    x: u8,
    tail: A,
}

struct Bar<T: ?Sized> {
    x: u8,
    tail: T,
}

fn assert_sized<T>() { }

fn main() {
    assert_sized::<A>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    assert_sized::<Foo>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    assert_sized::<Bar<A>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    assert_sized::<Bar<Bar<A>>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

