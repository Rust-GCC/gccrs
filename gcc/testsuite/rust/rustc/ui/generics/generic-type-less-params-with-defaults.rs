use std::marker;

struct Heap;

struct Vec<T, A = Heap>(
    marker::PhantomData<(T,A)>);

fn main() {
    let _: Vec;
// { dg-error ".E0107." "" { target *-*-* } .-1 }
}

