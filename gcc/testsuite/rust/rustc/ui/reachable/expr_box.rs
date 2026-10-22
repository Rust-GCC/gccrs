#![feature(box_syntax)]
#![allow(unused_variables)]
#![deny(unreachable_code)]

fn main() {
    let x = box return; // { dg-error "" "" { target *-*-* } }
    println!("hi");
}

