#![allow(unused)]

fn f() {
    let mut x: Vec<()> = Vec::new();

    || {
        || {
            x.push(())
        }
// { dg-error "" "" { target *-*-* } .-3 }
    };
}

fn main() {}

