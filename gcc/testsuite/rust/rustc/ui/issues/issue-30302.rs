#![allow(dead_code)]
#![allow(unused_variables)]
#![allow(non_snake_case)]
#![deny(unreachable_patterns)]

enum Stack<T> {
    Nil,
    Cons(T, Box<Stack<T>>)
}

fn is_empty<T>(s: Stack<T>) -> bool {
    match s {
        Nil => true,
// { dg-warning "" "" { target *-*-* } .-1 }
        _ => false
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

fn main() {}

