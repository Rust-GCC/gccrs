#![feature(type_ascription)]

fn main() {
    println!("test"):
    0; // { dg-error "" "" { target *-*-* } }
}

fn foo() {
    println!("test"): 0; // { dg-error "" "" { target *-*-* } }
}

