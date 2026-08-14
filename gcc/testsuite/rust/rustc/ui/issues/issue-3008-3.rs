use std::marker;

enum E1 { V1(E2<E1>), }
enum E2<T> { V2(E2<E1>, marker::PhantomData<T>), }
// { dg-error ".E0072." "" { target *-*-* } .-1 }

impl E1 { fn foo(&self) {} }

fn main() {
}

