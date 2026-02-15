#![allow(unused_variables)]
#![allow(unused_assignments)]
#![allow(dead_code)]
#![deny(unreachable_code)]

fn b() {
    match () { () => return }
    println!("I am dead");
// { dg-error "" "" { target *-*-* } .-1 }
}

fn c() {
    match () { () if false => return, () => () }
    println!("I am not dead");
}

fn d() {
    match () { () if false => return, () => return }
    println!("I am dead");
// { dg-error "" "" { target *-*-* } .-1 }
}

fn e() {
    // Here the compiler fails to figure out that the `println` is dead.
    match () { () if return => (), () => return }
    println!("I am dead");
}

fn f() {
    match Some(()) { None => (), Some(()) => return }
    println!("I am not dead");
}

fn g() {
    match Some(()) { None => return, Some(()) => () }
    println!("I am not dead");
}

fn main() { }

