#![feature(box_patterns)]

struct A;

fn main() {
    let a: Box<[A]> = Box::new([A]);
    match a { // { dg-error ".E0508." "" { target *-*-* } }
        box [a] => {},
        _ => {}
    }
}

