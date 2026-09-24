// run-rustfix

#![allow(unused)] // for rustfix

#[derive(Clone, Copy)]
struct S;

trait T {
    fn foo((x, y): (i32, i32)); // { dg-error ".E0642." "" { target *-*-* } }

    fn bar((x, y): (i32, i32)) {} // { dg-error ".E0642." "" { target *-*-* } }

    fn method(S { .. }: S) {} // { dg-error ".E0642." "" { target *-*-* } }

    fn f(&ident: &S) {} // ok
    fn g(&&ident: &&S) {} // ok
    fn h(mut ident: S) {} // ok
}

fn main() {}

