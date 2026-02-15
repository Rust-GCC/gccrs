#![allow(dead_code)]
#![forbid(box_pointers)]
#![feature(box_syntax)]

struct Foo {
    x: Box<isize> // { dg-error "" "" { target *-*-* } }
}

fn main() {
    let _x : Foo = Foo {x : box 10};
// { dg-error "" "" { target *-*-* } .-1 }
}

