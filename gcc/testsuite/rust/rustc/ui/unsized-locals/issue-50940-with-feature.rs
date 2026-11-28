#![feature(unsized_locals, unsized_fn_params)]
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {
    struct A<X: ?Sized>(X);
    A as fn(str) -> A<str>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

