// run-rustfix

#![allow(warnings)]

// This tests that the `help: consider dereferencing the boxed value` suggestion is made and works.

enum S {
    A { a: usize },
    B { b: usize },
}

enum T {
    A(usize),
    B(usize),
}

fn main() {
    let x = Box::new(T::A(3));
    let y = match x {
        T::A(a) | T::B(a) => a,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    };

    let x = Box::new(S::A { a: 3 });
    let y = match x {
        S::A { a } | S::B { b: a } => a,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    };
}

