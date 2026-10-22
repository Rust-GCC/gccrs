#![feature(box_syntax)]
#![allow(unused_variables)]
#![deny(unreachable_code)]

fn main() {
    let x = loop {
        continue;
        println!("hi");
// { dg-error "" "" { target *-*-* } .-1 }
    };
}

