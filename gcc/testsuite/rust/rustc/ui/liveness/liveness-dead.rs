#![allow(dead_code)]
#![deny(unused_assignments)]

fn f1(x: &mut isize) {
    *x = 1; // no error
}

fn f2() {
    let mut x: isize = 3; // { dg-error "" "" { target *-*-* } }
    x = 4;
    x.clone();
}

fn f3() {
    let mut x: isize = 3;
    x.clone();
    x = 4; // { dg-error "" "" { target *-*-* } }
}

fn f4(mut x: i32) { // { dg-error "" "" { target *-*-* } }
    x = 4;
    x.clone();
}

fn f5(mut x: i32) {
    x.clone();
    x = 4; // { dg-error "" "" { target *-*-* } }
}

// #22630
fn f6() {
    let mut done = false;
    while !done {
        done = true; // no error
        continue;
    }
}

fn main() {}

