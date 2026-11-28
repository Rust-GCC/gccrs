// check-pass
#![feature(min_const_generics)]
#![allow(dead_code)]

fn foo<T>() {
    [0; std::mem::size_of::<*mut T>()];
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

struct Foo<T>(T);

impl<T> Foo<T> {
    const ASSOC: usize = 4;

    fn test() {
        let _ = [0; Self::ASSOC];
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    }
}

struct Bar<const N: usize>;

impl<const N: usize> Bar<N> {
    const ASSOC: usize = 4;

    fn test() {
        let _ = [0; Self::ASSOC];
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    }
}

fn main() {}

