#![allow(unused_variables)]
#![allow(unused_assignments)]
#![allow(dead_code)]
#![deny(unreachable_code)]

fn foo() {
    while {return} {
// { dg-error "" "" { target *-*-* } .-1 }
        println!("Hello, world!");
    }
}

fn bar() {
    while {true} {
        return;
    }
    println!("I am not dead.");
}

fn baz() {
    // Here, we cite the `while` loop as dead.
    while {return} {
// { dg-error "" "" { target *-*-* } .-1 }
        println!("I am dead.");
    }
    println!("I am, too.");
}

fn main() { }

