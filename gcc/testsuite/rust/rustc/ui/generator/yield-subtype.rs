// run-pass
#![allow(dead_code)]
#![allow(dead_code)]

#![feature(generators)]

fn bar<'a>() {
    let a: &'static str = "hi";
    let b: &'a str = a;

    || { // { dg-warning "" "" { target *-*-* } }
        yield a;
        yield b;
    };
}

fn main() {}

