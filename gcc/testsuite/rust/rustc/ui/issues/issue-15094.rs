#![feature(fn_traits, unboxed_closures)]

use std::{fmt, ops};

struct Debuger<T> {
    x: T
}

impl<T: fmt::Debug> ops::FnOnce<(),> for Debuger<T> {
    type Output = ();
    fn call_once(self, _args: ()) {
// { dg-error ".E0053." "" { target *-*-* } .-1 }
// { dg-error ".E0053." "" { target *-*-* } .-2 }
// { dg-error ".E0053." "" { target *-*-* } .-3 }
        println!("{:?}", self.x);
    }
}

fn make_shower<T>(x: T) -> Debuger<T> {
    Debuger { x: x }
}

pub fn main() {
    let show3 = make_shower(3);
    show3();
}

