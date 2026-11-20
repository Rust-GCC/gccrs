// This test checks that genuine type errors with partial
// type hints are understandable.

use std::marker::PhantomData;

struct Foo<T>(PhantomData<T>);
struct Bar<U>(PhantomData<U>);

pub fn main() {
}

fn test1() {
    let x: Foo<_> = Bar::<usize>(PhantomData);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
    let y: Foo<usize> = x;
}

fn test2() {
    let x: Foo<_> = Bar::<usize>(PhantomData);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
}

