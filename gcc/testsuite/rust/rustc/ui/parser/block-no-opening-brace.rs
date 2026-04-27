// edition:2018

#![feature(try_blocks)]

fn main() {}

fn f1() {
    loop
        let x = 0; // { dg-error "" "" { target *-*-* } }
        drop(0);
    }

fn f2() {
    while true
        let x = 0; // { dg-error "" "" { target *-*-* } }
    }

fn f3() {
    for x in 0..1
        let x = 0; // { dg-error "" "" { target *-*-* } }
    }

fn f4() {
    try // { dg-error "" "" { target *-*-* } }
        let x = 0;
    }

fn f5() {
    async // { dg-error ".E0658." "" { target *-*-* } }
        let x = 0; // { dg-error "" "" { target *-*-* } }
    }

