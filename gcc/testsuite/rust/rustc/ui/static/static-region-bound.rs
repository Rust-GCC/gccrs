#![feature(box_syntax)]

fn id<T>(x: T) -> T { x }

fn f<T:'static>(_: T) {}

fn main() {
    let x: Box<_> = box 3;
    f(x);
    let x = &id(3); // { dg-error ".E0716." "" { target *-*-* } }
    f(x);
}

