#![allow(unused)]

fn f() {
    let mut x: Box<()> = Box::new(());

    || {
        &mut x
    };
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}

