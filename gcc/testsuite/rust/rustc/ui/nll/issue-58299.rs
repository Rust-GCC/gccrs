#![feature(nll)]

struct A<'a>(&'a ());

trait Y {
    const X: i32;
}

impl Y for A<'static> {
    const X: i32 = 10;
}

fn foo<'a>(x: i32) {
    match x {
        // This uses <A<'a> as Y>::X, but `A<'a>` does not implement `Y`.
        A::<'a>::X..=A::<'static>::X => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }
}

fn bar<'a>(x: i32) {
    match x {
        // This uses <A<'a> as Y>::X, but `A<'a>` does not implement `Y`.
        A::<'static>::X..=A::<'a>::X => (), // { dg-error "" "" { target *-*-* } }
        _ => (),
    }
}

fn main() {}

