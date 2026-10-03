#![deny(unused_must_use)]

#[must_use]
struct S;

struct A;

#[must_use]
trait T {}

impl T for A {}

fn empty() -> [S; 0] {
    []
}

fn singleton() -> [S; 1] {
    [S]
}

fn many() -> [S; 4] {
    [S, S, S, S]
}

fn array_of_impl_trait() -> [impl T; 2] {
    [A, A]
}

fn impl_array() -> [(u8, Box<dyn T>); 2] {
    [(0, Box::new(A)), (0, Box::new(A))]
}

fn array_of_arrays_of_arrays() -> [[[S; 1]; 2]; 1] {
    [[[S], [S]]]
}

fn main() {
    empty(); // ok
    singleton(); // { dg-error "" "" { target *-*-* } }
    many(); // { dg-error "" "" { target *-*-* } }
    ([S], 0, ()); // { dg-error "" "" { target *-*-* } }
    array_of_impl_trait(); // { dg-error "" "" { target *-*-* } }
    impl_array();
// { dg-error "" "" { target *-*-* } .-1 }
    array_of_arrays_of_arrays();
// { dg-error "" "" { target *-*-* } .-1 }
}

