#![feature(unsized_locals)]
// { dg-warning "" "" { target *-*-* } .-1 }

struct A;
#[derive(Clone, Copy)]
struct C;

fn main() {
    let a: Box<[A]> = Box::new([A]);
    match *a {
// { dg-error ".E0508." "" { target *-*-* } .-1 }
        [a @ ..] => {}
        _ => {}
    }
    let b: Box<[A]> = Box::new([A, A, A]);
    match *b {
// { dg-error ".E0508." "" { target *-*-* } .-1 }
        [_, _, b @ .., _] => {}
        _ => {}
    }

    // `[C]` isn't `Copy`, even if `C` is.
    let c: Box<[C]> = Box::new([C]);
    match *c {
// { dg-error ".E0508." "" { target *-*-* } .-1 }
        [c @ ..] => {}
        _ => {}
    }
    let d: Box<[C]> = Box::new([C, C, C]);
    match *d {
// { dg-error ".E0508." "" { target *-*-* } .-1 }
        [_, _, d @ .., _] => {}
        _ => {}
    }
}

