#![allow(unused_variables)]
#![allow(unused_assignments)]
#![allow(dead_code)]
#![deny(unreachable_code)]

fn a() {
    loop { return; }
    println!("I am dead.");
// { dg-error "" "" { target *-*-* } .-1 }
}

fn b() {
    loop {
        break;
    }
    println!("I am not dead.");
}

fn c() {
    loop { return; }
    println!("I am dead.");
// { dg-error "" "" { target *-*-* } .-1 }
}

fn d() {
    'outer: loop { loop { break 'outer; } }
    println!("I am not dead.");
}

fn e() {
    loop { 'middle: loop { loop { break 'middle; } } }
    println!("I am dead.");
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() { }

