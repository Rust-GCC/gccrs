#![feature(unsized_locals)]
// { dg-warning "" "" { target *-*-* } .-1 }

struct Test([i32]);

fn main() {
    let _x: fn(_) -> Test = Test;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

